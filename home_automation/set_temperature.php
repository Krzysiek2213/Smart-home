<?php
if (isset($_POST['setTemperature'])) {
    $setTemperature = floatval($_POST['setTemperature']);
    $url = 'http://192.168.6.205/temperature?set=' . $setTemperature;
    file_get_contents($url);
}
header('Location: index.php');
?>
