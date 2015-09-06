	static char hdr[]="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
						"<html><body><span style=\"color:#0000A0\">\r\n"
						"<h1><center>Olduino 1802 Blinkenlights</center></h1>\r\n";

	static char postform[]="<p><form method=\"POST\">\r\n"
						"<input type=\"submit\" value=\"Toggle LED\">\r\n"
						"</form>";
	static char getform[]="<p><form method=\"GET\">\r\n"
						"<input type=\"submit\" value=\"LED Status\">\r\n"
						"</form>";
	static unsigned char olduinolink[]="<a href=\"http://goo.gl/RYbPYC\">Olduino</a>: An Arduino for the First of Us<p>";
	static char trlr[]="</body></html>\r\n\r\n";
	static char gifon[]="<img src=\"http://goo.gl/xBeA8S\">\r\n";//http://bit.ly/1LAGSm5
	static char gifoff[]="<img src=\"http://goo.gl/l2xONS\">\r\n";//http://bit.ly/1EiEYnx
	unsigned char tweetflag=0,tweetstate=0;
