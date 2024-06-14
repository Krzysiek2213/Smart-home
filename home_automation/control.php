<?php
if (isset($_GET['light']) && isset($_GET['state'])) {
    $light = $_GET['light'];
    $state = $_GET['state'];

    $url = 'http://192.168.6.205/?' . $light . $state;
    file_get_contents($url);
}
header('Location: index.php');
?>
