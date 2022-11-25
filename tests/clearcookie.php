<?php
	// clear cookie
	setcookie("test", "", time()-3600);
	// stop keep alive
	// set header
	header("Connection: close");
	// echo "done";
?>