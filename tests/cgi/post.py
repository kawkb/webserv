import cgi, cgitb

# get first name and last name from form
form = cgi.FieldStorage()
first_name = form.getvalue('first_name')
last_name = form.getvalue('last_name')

# print the response
print ("hello %s %s" % (first_name, last_name))