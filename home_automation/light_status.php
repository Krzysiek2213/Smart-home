<?php
$response = file_get_contents('http://192.168.6.205/light_status');
$data = json_decode($response, true);
echo json_encode($data);
?>
