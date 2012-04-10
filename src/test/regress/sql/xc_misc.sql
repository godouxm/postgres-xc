-- A function to create table on specified nodes 
create or replace function cr_table(tab_schema varchar, nodenums int[], distribution varchar) returns void language plpgsql as $$
declare
	cr_command	varchar;
	nodes		varchar[];
	nodename	varchar;
	nodenames_query varchar;
	nodenames 	varchar;
	node 		int;
	sep			varchar;
	tmp_node	int;
	num_nodes	int;
begin
	nodenames_query := 'SELECT node_name FROM pgxc_node WHERE node_type = ''D'''; 
	cr_command := 'CREATE TABLE ' || tab_schema || ' DISTRIBUTE BY ' || distribution || ' TO NODE ';
	for nodename in execute nodenames_query loop
		nodes := array_append(nodes, nodename);
	end loop;
	nodenames := '';
	sep := '';
	num_nodes := array_length(nodes, 1);
	foreach node in array nodenums loop
		tmp_node := node;
		if (tmp_node < 1 or tmp_node > num_nodes) then
			tmp_node := tmp_node % num_nodes;
			if (tmp_node < 1) then
				tmp_node := num_nodes; 
			end if;
		end if;
		nodenames := nodenames || sep || nodes[tmp_node];
		sep := ', ';
	end loop;
	cr_command := cr_command || nodenames;
	execute cr_command;
end;
$$;

-- A function to return a unified data node name given a node identifer 
create or replace function get_unified_node_name(node_ident int) returns varchar language plpgsql as $$
declare
	r pgxc_node%rowtype;
	node int;
	nodenames_query varchar;
begin
	nodenames_query := 'SELECT * FROM pgxc_node  WHERE node_type = ''D'' ORDER BY xc_node_id';

	node := 1;
	for r in execute nodenames_query loop
		if r.node_id = node_ident THEN
			RETURN 'NODE_' || node;
		end if;
		node := node + 1;
	end loop;
	RETURN 'NODE_?';
end;
$$;

-- Test the system column added by XC called xc_node_id, used to find which tuples belong to which data node

select cr_table('t1(a int, b int)', '{1, 2}'::int[], 'modulo(a)');
insert into t1 values(1,11),(2,11),(3,11),(4,22),(5,22),(6,33),(7,44),(8,44);

select get_unified_node_name(xc_node_id),* from t1 order by a;

select get_unified_node_name(xc_node_id),* from t1 where xc_node_id > 0 order by a;

select get_unified_node_name(xc_node_id),* from t1 order by xc_node_id;

create table t2(a int , xc_node_id int) distribute by modulo(a);

create table t2(a int , b int) distribute by modulo(xc_node_id);

drop table t1;

