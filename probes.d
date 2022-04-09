BEGIN
{
	printf("dtrace has started tracking your actions!!!");
	start = timestamp;
}

/*
syscall::openat:entry
{
	chill(10000);
}

*/
/*
syscall:::entry
/ pid != $pid /
{

	printf("process with the process id: %d , name: %s is making a system call \n", pid, execname);

}

syscall::write:entry
{
	printf("process with the process id: %d , name: %s is writing to a file \n", pid, execname);
}

syscall::open*:entry
{
	printf("%s %s\n", execname, copyinstr(arg0));
}

proc:::exec-success
{
	trace(curpsinfo->pr_psargs);
}

sched:::off-cpu
{
	@[stack()] = count();
}

io:::start
{
	@[stack()] = count();
}

ip:::
{
	@[probename] = count();
}
*/

/*
vfs:namecache:lookup:
{
	@missing[execname] = count();
}

*/

/*
lockstat:::adaptive-block
{
	@[arg0] = sum(arg1);
}

END 
{
	printa("%40a %@16d ns\n", @);
}

*/

fbt::kmem*:entry
{
	@[probefunc] = count();
}

END 
{
	printf("total time spent tracing: %d secs", (timestamp-start)/1000000000);
}
