#start server on port 8081
./webserv &  
#try to  ping  localhost on port 8081 
#send a POST request to localhost on port 8081 with a header of content-length: 0
echo "Posting to localhost:8081 with a body size of 0\n\n "
sleep 1
nl -ba -w10 -s" " /dev/zero | head -c 0 | curl -v -X POST -H "Content-Length: 0" http://localhost:8081
sleep 1 
echo "-----------------------------------------------\n\n "

echo "Posting to localhost:8081 with a body size  too big"
#send a POST request to localhost on port 8081 with  a header of lenght too big to be handled by the server
nl -ba -w10 -s" " /dev/zero | head -c 0 | curl -v -X POST -H "Content-Length: 8888888888888888888888888888" http://localhost:8081
sleep 1 
echo "-----------------------------------------------\n\n"


echo "GET request on a wrong path"
nl -ba -w10 -s" " /dev/zero | head -c 0 | curl -v -X GET  http://localhost:8081/rat
sleep 1 
echo "-----------------------------------------------\n\n"

echo "Posting to localhost:8081 with incorrect request option"
#send a POST request to localhost on port 8081 with  a header of lenght too big to be handled by the server
nl -ba -w10 -s" " /dev/zero | head -c 0 | curl -v -X SHIT  http://localhost:8081

sleep 1 
echo "-----------------------------------------------\n\n"
echo "GET request to localhost:8081 to show directory listing"
#send a POST request to localhost on port 8081 with  a header of lenght too big to be handled by the server
nl -ba -w10 -s" " /dev/zero | head -c 20 | curl -v -X GET  http://localhost:8081/upload  

sleep 1 
echo "-----------------------------------------------\n\n"

echo "DELETE to localhost:8081"
find  ./resources | grep upload 
nl -ba -w10 -s" " /dev/zero | head -c 20 | curl -v -X DELETE  http://localhost:8081/
find  ./resources | grep upload
sleep 1
echo "-----------------------------------------------\n\n"


echo "GET request on another instance of the server" 
find  ./resources | grep upload 
nl -ba -w10 -s" " /dev/zero | head -c 20 | curl -v -X GET  http://localhost:8082/
sleep 1
echo "-----------------------------------------------\n\n"




#kill the server
kill %1

