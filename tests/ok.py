import os
import cgi
import sys
form = cgi.FieldStorage()
f_name=form["f_name"].value
s_name=form["s_name"].value
print(f_name)
print(s_name)
# print "Content-type: text/html