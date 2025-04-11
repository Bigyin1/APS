var ws;
var connected = false;
let gpio = Object();

function setLED(led, state) {
    led.querySelector("circle").setAttribute('opacity', state ? 1.0 : 0.1);
}

function doConnect(addr) {
    ws = new WebSocket(addr);

    /* Register events. */
    ws.onopen = function () {
        connected = true;
        document.getElementById("btConn").value = "Disconnect";

        const nexysObject = document.getElementById('nexys-svg')
        const nexys = nexysObject.contentDocument;

        var msg = { gpio: {} }
        for (let i = 0; i < 16; i++) {
            var sw = nexys.getElementById('SW' + i);
            const circle = sw.querySelector("circle")
            const cy = circle.getAttribute('cy')
            const is_on = 0;

            circle.setAttribute('cy', is_on ? 850 : 900);

            const s = 'SW' + i;
            msg.gpio[s] = is_on == 1;
        }


        ws.send(JSON.stringify(msg));
    };

    document.getElementById("btConn").onclick = function () {
        if (connected == false) {
            doConnect("ws://localhost:8081");
        } else {
            ws.close();
        }
    };

    document.getElementById("uartClear").onclick = function () {
        document.getElementById("uart").value = "";
    };

    /* Deals with messages. */
    ws.onmessage = function (evt) {
        console.log(evt.data);
        const json_obj = JSON.parse(evt.data);
        if ("gpio" in json_obj) {
            for (const [key, value] of Object.entries(json_obj.gpio)) {
                if (key in gpio) {
                    /* Check if key exists */
                    setLED(gpio[key], value);
                } else {
                    console.log("Missing key");
                }
            }
        }
        if ("serial" in json_obj) {
            uart_area = document.getElementById("uart");
            uart_area.value += String.fromCharCode(json_obj.serial.uart);
            uart_area.scrollTop = uart_area.scrollHeight;
        }

        if ("SevSegDispl0" in json_obj) {
            document.getElementById("SevSegDispl0").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl0");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl0);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl1" in json_obj) {
            document.getElementById("SevSegDispl1").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl1");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl1);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl2" in json_obj) {
            document.getElementById("SevSegDispl2").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl2");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl2);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl3" in json_obj) {
            document.getElementById("SevSegDispl3").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl3");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl3);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl4" in json_obj) {
            document.getElementById("SevSegDispl4").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl4");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl4);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl5" in json_obj) {
            document.getElementById("SevSegDispl5").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl5");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl5);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl6" in json_obj) {
            document.getElementById("SevSegDispl6").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl6");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl6);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

        if ("SevSegDispl7" in json_obj) {
            document.getElementById("SevSegDispl7").value = "";
            SevSegDispl_area = document.getElementById("SevSegDispl7");
            SevSegDispl_area.value += String.fromCharCode(json_obj.SevSegDispl7);
            SevSegDispl_area.scrollTop = SevSegDispl_area.scrollHeight;
        }

    };

    /* Close events. */
    ws.onclose = function (event) {
        document.getElementById("btConn").value = "Connect to board";
        connected = false;
    };
}

document.addEventListener("DOMContentLoaded", function (event) {
    document.getElementById("btConn").onclick = function () {
        if (connected == false) {
            doConnect("ws://localhost:8081");
        } else {
            ws.close();
        }
    };
});

const nexysObject = document.getElementById('nexys-svg')
nexysObject.addEventListener("load", function () {
    const nexys = nexysObject.contentDocument;
    for (let i = 0; i < 16; i++) {
        nexys.getElementById('SW' + i).onclick = toggleSwitch;
        gpio['LD' + i] = nexys.getElementById('LD' + i);
    }

    for (let i = 16; i < 21; i++) {
        nexys.getElementById('SW' + i).onclick = toggleSwitch16to20;
    }

    function toggleSwitch() {
        const circle = this.querySelector("circle")
        const cy = circle.getAttribute('cy')
        const is_on = cy == 900;

        circle.setAttribute('cy', is_on ? 850 : 900);

        const s = new String(this.getAttribute('id'));

        var msg = { gpio: { [s]: is_on } };

        ws.send(JSON.stringify(msg));
    }

    function toggleSwitch16to20() {
        const circle = this.querySelector("circle")
        const r = circle.getAttribute('r')
        const is_on = r == 18;

        circle.setAttribute('r', is_on ? 10 : 18);

        const s = new String(this.getAttribute('id'));

        var msg = { gpio: { [s]: is_on } };

        ws.send(JSON.stringify(msg));
    }


}, false);