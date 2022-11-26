
<?php
	// set cookie
	// get cookie value from request body
	// $cookie = $_POST["cookie"];
	// get from query
	// get query
	$cookie = $_GET["cookie"];
	echo $cookie;
	setcookie("test", $cookie, time()+3600);
	// echo "ell";
?>