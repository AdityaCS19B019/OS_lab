proc:::start
/ gid == $1 /
{
	self->start = timestamp;
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
	printf("gid: %d, uid: %d, pid: %d, program name: %s, file name: %s \n \n", gid, uid, pid, execname, copyinstr(str0));
}
 
syscall::close:return
/ gid == $1 /
{
	printf("the process %s with the pid %d  has opened the file named %s for %d sec\n\n", execname, pid, copyinstr(arg0), (timestamp-start)/1000000000);
}

proc:::exit
/ gid == $1 && self->start /
{
	@[execname] = quantize(timestamp-self->start);
	self->start = 0;
	printf("\n\n");
}

