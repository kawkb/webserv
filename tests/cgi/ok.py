import cgi, cgitb
import sys
from time import sleep

# Create instance of FieldStorage 
form = cgi.FieldStorage() 
# get query string
values = cgi.parse()

print ("Content-type: text/html\r\n\r\n")
print ("hello " + values['name'][0] + "!")
