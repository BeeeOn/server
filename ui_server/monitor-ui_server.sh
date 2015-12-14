
openssl s_client  -connect  ant-2.fit.vutbr.cz:4566 -key ./cert/ant-2.fit.vutbr.cz.key <<< "<com/>"


$log="log"
$compressedLog="log.bz"

echo compressing $log to $compressedLog
bzip2 < log  > comp2
 
echo sending mail
echo "ahoj" |  mail -s "test" -a $compressedLog xvampo01@stud.fit.vutbr.cz

echo done