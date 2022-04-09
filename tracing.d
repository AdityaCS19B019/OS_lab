proc:::exec
/ gid == $1 /
{
	printf("%s \n\n", execname);
	/* self->start = timestamp; */
}

/*
syscall::read:
{

	printf("%s	%s \n\n", execname, stringof(copyin(arg1, arg2)));
}
*/

syscall::write:entry
/ gid == $1 /
{
	printf("%s\n\n", copyinstr(arg1));
}

syscall:::entry
/ gid == $1 /
{
	@[execname] = count();

	printf("\n\n");
}

syscall::open:entry
/ gid == $1 /
{
	start = timestamp;
	printf("gid: %d, uid: %d, pid: %d, program name: %s, file name: %s \n \n", gid, uid, pid, execname, copyinstr(arg0));
}
 
syscall::close:entry
/ gid == $1 /
{
	printf("the process %s with the pid %d  has opened a file for %d sec\n\n", execname, pid, (timestamp-start)/1000000000);
}

proc:::exit
/ gid == $1 /
{
/*	@[execname] = quantize(timestamp-self->start);
	self->start = 0;   */
	printf("\n\n");
}

