#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
if form.getvalue('textcontent'):
   text_content = form.getvalue('textcontent')
else:
   text_content = "Not entered"

print "<html>"
print "<head>"
print "<title>Text Area CGI Program</title>"
print "</head>"
print "<body>"
print "<form action='post.py' method='post' target='_blank'>"
print " <textarea name='textcontent' cols='40' rows='4'> "
print " </textarea>"
print " <input type = 'submit' value = 'Submit' />"
print "</form>"
print "<h2> Entered Text Content is  :  %s</h2>" % text_content
print "</body>"