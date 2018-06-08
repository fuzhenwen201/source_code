#!/bin/sh

function backup_log()
{
	backup_dir=$1
	
	if [ `ls *.log |wc -l` != 0 ]
	then
		#create a dir
		mkdir -p  ${backup_dir}
		#cp log files
		mv --backup=t *.log  ${backup_dir}
		
		echo "`date`-> back log to ${backup_dir}" >> $2
		return 1
	else
		echo "`date`-> log files is not exist" >> $2
		return 0
	fi
}

# change to the log path

log_dir=${APPHOME}/log
E_XCD=66 # can not change dir, error num!
cd ${log_dir}

if [ `pwd` != "${log_dir}" ] 
then
	echo "Can't change to ${log_dir}."
	exit $E_XCD
fi

backup_log_file="backup_log.txt"

# get shell scripts name
my_name=$0
my_name=${my_name:2:`expr ${#my_name} - 1`}
echo "${myName} start to backup log at `date`"  >> ${backup_log_file}

# get current date
# lastTime=`date +%Y%m%d%H%M`
# curTime=`date +%Y%m%d%H%M`

last_time=`date +%Y%m%d%H`
cur_time=`date +%Y%m%d%H`

# backup count num
backup_count=0

while true
do
	sleep 1m
	echo "`date`-> is a live" >> ${backup_log_file}
	cur_time=`date +%Y%m%d%H`
	
	if [ ${last_time} != ${cur_time} ]
	then
		# call a function input para is backup dir and shell log dir
		backup_log "backup${last_time}" ${backup_log_file}
		if [ $? == 1 ]
		then
			backup_count=`expr ${backup_count} + 1`
			echo "`date`-> current backcount is ${backup_count}" >> ${backup_log_file}
		fi
		last_time=${cur_time}
	fi
done

exit 0