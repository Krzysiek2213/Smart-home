<!DOCTYPE html>
<html>
<head>
    <title>Home Automation</title>
    <link rel="stylesheet" type="text/css" href="css/styles.css">
    <script>
        function updateTimeAndDate() {
            const now = new Date();
            const time = now.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit' });
            const date = now.toLocaleDateString('en-US', { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' });
            document.querySelector('.user-info .time').innerText = time;
            document.querySelector('.user-info .date').innerText = date;
        }

        function updateStatus() {
            updateTimeAndDate();

            fetch('temperature_status.php')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('currentTemperature').innerText = data.currentTemperature + ' °C';
                    document.getElementById('setTemperature').value = data.setTemperature;
                    document.getElementById('heaterStatus').innerText = data.heaterState ? 'ON' : 'OFF';
                })
                .catch(error => console.error('Error fetching temperature status:', error));

            fetch('motion_status.php')
                .then(response => response.text())
                .then(data => {
                    if (data === '1') {
                        document.getElementById('motionAlert').innerText = 'Motion detected!';
                    } else {
                        document.getElementById('motionAlert').innerText = '';
                    }
                })
                .catch(error => console.error('Error fetching motion status:', error));

            fetch('fire_status.php')
                .then(response => response.text())
                .then(data => {
                    console.log('Fire status:', data); // Debugging line
                    if (data === '1') {
                        document.getElementById('fireStatus').innerText = 'Fire detected!';
                    } else {
                        document.getElementById('fireStatus').innerText = '';
                    }
                })
                .catch(error => console.error('Error fetching fire status:', error));

            fetch('light_status.php')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('kitchenStatus').innerText = data.kitchen ? 'ON' : 'OFF';
                    document.getElementById('bedroomStatus').innerText = data.bedroom ? 'ON' : 'OFF';
                    document.getElementById('officeStatus').innerText = data.office ? 'ON' : 'OFF';
                    document.getElementById('livingroomStatus').innerText = data.livingroom ? 'ON' : 'OFF';
                })
                .catch(error => console.error('Error fetching light status:', error));
        }

        setInterval(updateStatus, 1000); // Update every second
    </script>
</head>
<body onload="updateStatus()">
    <div class="sidebar">
        <div class="sidebar-header">
            <img src="house_image.png" alt="House Image">
            <h2>Menu</h2>
        </div>
        <nav class="sidebar-nav">
            <a href="index.html" class="nav-item active"><span class="icon home-icon"></span> Home</a>
            <a href="livingroom.html" class="nav-item"><span class="icon rooms-icon"></span> Living Room</a>
            <a href="bedroom.html" class="nav-item"><span class="icon rooms-icon"></span> Bedroom</a>
            <a href="kitchen.html" class="nav-item"><span class="icon rooms-icon"></span> Kitchen</a>
            <a href="office.html" class="nav-item"><span class="icon rooms-icon"></span> Office</a>
            <a href="#" class="nav-item"><span class="icon security-icon"></span> Security</a>
            <!--<a href="#" class="nav-item"><span class="icon cctv-icon"></span> CCTV</a>-->
            <a href="#" class="nav-item"><span class="icon settings-icon"></span> Settings</a>
        </nav>
    </div>
    <div class="content">
        <header>
            <input type="text" placeholder="Type something">
            <div class="user-info">
                <span class="time"></span>
                <span class="date"></span>
                <span class="temp">+15°C</span>
                <img src="user_avatar.png" alt="User Avatar" class="user-avatar">
                <span class="notifications-icon"></span>
            </div>
        </header>
        <main>
            <section class="greeting">
                <h1>Hi, User!</h1>
                <p>Welcome, home! Great, weather today - sun shining and a refreshing breeze</p>
            </section>
            <section class="controls">
                <h2>Rooms</h2>
                <div class="rooms-grid">
                    <div class="room-card">
                        <p>Living Room: <span id="livingroomStatus"></span></p>
                        <button onclick="location.href='control.php?light=livingroom&state=on'">Turn On</button>
                        <button onclick="location.href='control.php?light=livingroom&state=off'">Turn Off</button>
                    </div>
                    <div class="room-card">
                        <p>Bedroom: <span id="bedroomStatus"></span></p>
                        <button onclick="location.href='control.php?light=bedroom&state=on'">Turn On</button>
                        <button onclick="location.href='control.php?light=bedroom&state=off'">Turn Off</button>
                    </div>
                    <div class="room-card">
                        <p>Kitchen: <span id="kitchenStatus"></span></p>
                        <button onclick="location.href='control.php?light=kitchen&state=on'">Turn On</button>
                        <button onclick="location.href='control.php?light=kitchen&state=off'">Turn Off</button>
                    </div>
                    <div class="room-card">
                        <p>Office: <span id="officeStatus"></span></p>
                        <button onclick="location.href='control.php?light=office&state=on'">Turn On</button>
                        <button onclick="location.href='control.php?light=office&state=off'">Turn Off</button>
                    </div>
                </div>
            </section>
            <section class="temperature-control">
                <h2>Temperature Control</h2>
                <p>Current Temperature: <span id="currentTemperature"></span></p>
                <p>Heating is: <span id="heaterStatus"></span></p>
                <form action="set_temperature.php" method="post">
                    Set Temperature: <input type="number" name="setTemperature" id="setTemperature" step="0.1">
                    <input type="submit" value="Set">
                </form>
            </section>
        </main>
    </div>
</body>
</html>
