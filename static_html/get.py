import cgi, cgitb
import sys
from time import sleep

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')


print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<form action = 'get.py' method = 'get'>")
print ("First Name: <input type = 'text' name = 'first_name'>  <br />")
print ("Last Name: <input type = 'text' name = 'last_name' />")
print ("<input type = 'submit' value = 'Submit' />")
print ("</form>")
print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
print ("</body>")
print ("</html>")