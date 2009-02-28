#!/bin/sh
(automake --version) < /dev/null > /dev/null 2>&1 || 
{
	echo;
	echo "You must have automake installed";
	echo;
	exit;
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || 
{
	echo;
	echo "You must have autoconf installed";
	echo;
	exit;
}

echo "Generating configuration files for fxcalc...."
echo;

exec_cmd()
{
	echo executing $* ...;
	if ! $*; then
		echo "failed!";
		exit;
	fi
}

exec_cmd aclocal;
exec_cmd automake --add-missing --copy;
exec_cmd autoconf;

