    // Create ros object to communicate over your Rosbridge connection
    const ros = new ROSLIB.Ros({
        url: 'ws://localhost:9090',
        options: {
            ros_domain_id: '1' // ROS_DOMAIN_IDを設定
        }
    });

    // When the Rosbridge server connects, fill the span with id "status" with "successful"
    ros.on("connection", () => {
        document.getElementById("status").innerHTML = "接続成功";
        document.getElementById("status").style.color = "blue";
        console.log('Connected to ROSBridge WebSocket server.');
    });

    // When the Rosbridge server experiences an error, fill the "status" span with the returned error
    ros.on('error', function(error) {
        console.log('Error connecting to ROSBridge WebSocket server: ', error);
    });

    // When the Rosbridge server shuts down, fill the "status" span with "closed"
    ros.on('close', function() {
        console.log('Connection to ROSBridge WebSocket server closed.');
    });


    const joy = new ROSLIB.Topic({
        ros: ros,
        name: '/joy',
        messageType: 'sensor_msgs/Joy'
    });
/*
    let ros_data = new ROSLIB.Message({
        linear: { x: 0, y: 0, z: 0 },
        angular: { x: 0, y: 0, z: 0 }
    });
*/

    let ros_data = new ROSLIB.Message({
        header: {stamp : {sec : 0, nanosec : 0}, frame_id : 'Joy'},
        buttons: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        axes: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    });


	setInterval(function(){
        // 最新の Gamepad のリストを取得する
		var gamepads = navigator.getGamepads();
        var gamepad = gamepads[0];

        // Gamepad オブジェクトが存在する
        if(gamepad){
/*
            a.push("PLAYER:" + (i));
            a.push("index:" + (gamepad.index));
            a.push("timestamp:" + (gamepad.timestamp));
            a.push("id: \"" + (gamepad.id) + "\"");
            a.push("connected: " + (gamepad.connected));
            a.push("mapping: \"" + (gamepad.mapping) + "\"");

*/
            // ボタン情報
            var buttons = gamepad.buttons;
            var n = buttons.length;
            var L3 = (buttons[ 6].value * -2.0) + 1.0;
            var R3 = (buttons[ 7].value * -2.0) + 1.0;
            var Dpad_LR = 0.0;
            var Dpad_UD = 0.0;
            
            if((buttons[12].pressed==true )&&(buttons[13].pressed==false)){
                Dpad_UD = 1.0;
            }
            if((buttons[12].pressed==false)&&(buttons[13].pressed==true )){
                Dpad_UD =-1.0;
            }
            if((buttons[14].pressed==true )&&(buttons[15].pressed==false)){
                Dpad_LR = 1.0;
            }
            if((buttons[14].pressed==false)&&(buttons[15].pressed==true )){
                Dpad_LR =-1.0;
            }
//            console.log(buttons[0]);
            ros_data.buttons[ 0] = buttons[ 0].value;
            ros_data.buttons[ 1] = buttons[ 1].value;
            ros_data.buttons[ 2] = buttons[ 3].value;
            ros_data.buttons[ 3] = buttons[ 2].value;
            ros_data.buttons[ 4] = buttons[ 4].value;
            ros_data.buttons[ 5] = buttons[ 5].value;
            ros_data.buttons[ 6] = Boolean(buttons[ 6].value);
            ros_data.buttons[ 7] = Boolean(buttons[ 7].value);
            ros_data.buttons[ 8] = buttons[ 8].value;
            ros_data.buttons[ 9] = buttons[ 9].value;
            ros_data.buttons[10] = buttons[16].value;
            ros_data.buttons[11] = buttons[10].value;
            ros_data.buttons[12] = buttons[11].value;
//            ros_data.buttons[13] = buttons[17];
            var axes = gamepad.axes;
            var n = axes.length;
            ros_data.axes[ 0] = -1.0 * axes[0];
            ros_data.axes[ 1] = -1.0 * axes[1];
            ros_data.axes[ 2] = L3;
            ros_data.axes[ 3] = -1.0 * axes[2];
            ros_data.axes[ 4] = -1.0 * axes[3];
            ros_data.axes[ 5] = R3;
            ros_data.axes[ 6] = Dpad_LR;
            ros_data.axes[ 7] = Dpad_UD;
		}
        console.log(ros_data.axes[ 0]);
        joy.publish(ros_data);

	},1000/60);