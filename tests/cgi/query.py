import cgi, cgitb

# get first name and last name from query string
query = cgi.parse()
first_name = query['first_name']
last_name = query['last_name']
# print the response
print ("hello %s %s" % (first_name[0], last_name[0]))