if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep daemon|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep daemon | awk '{print $1}'|xargs kill -9
echo "kill daemon sucess"
else
echo "daemon not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$8}' | grep server|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep server | awk '{print $1}'|xargs kill -9
echo "kill server sucess"
else
echo "server not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep work|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep work | awk '{print $1}'|xargs kill -9
echo "kill work sucess"
else
echo "work not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep comclihttp|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep comclihttp | awk '{print $1}'|xargs kill -9
echo "kill comclishttp sucess"
else
echo "comclishttp not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep locsvr|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep locsvr | awk '{print $1}'|xargs kill -9
echo "kill locsvr sucess"
else
echo "locsvr not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep loccli|wc -l` != 0 ] 
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep loccli | awk '{print $1}'|xargs kill -9
echo "kill loccli sucess"
else
echo "loccli not running"
fi


if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep onlineservice|wc -l` != 0 ]
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep onlineservice | awk '{print $1}'|xargs kill -9
echo "kill onlineservice sucess"
else
echo "onlineservice not running"
fi


if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep httpclient|wc -l` != 0 ]
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep httpclient | awk '{print $1}'|xargs kill -9
echo "kill httpclient sucess"
else
echo "httpclient not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep worknew|wc -l` != 0 ]
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep worknew | awk '{print $1}'|xargs kill -9
echo "kill worknew sucess"
else
echo "worknew not running"
fi

if [ `ps -ef|awk /^sendy/'{print $2,$(8)}' | grep delhismarket|wc -l` != 0 ]
then
ps -ef|awk /^sendy/'{print $2,$(8)}' | grep delhismarket | awk '{print $1}'|xargs kill -9
echo "kill delhismarket sucess"
else
echo "delhismarket not running"
fi

