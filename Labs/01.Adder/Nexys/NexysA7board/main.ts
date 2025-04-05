import { Board } from "./board.js";
import { GPIO, BoardUpdates } from "./models.js";
import { EventBus } from "./pubsub.js";


class NexysClient {
    private ws: WebSocket;

    private connected = false;
    private connBtn: HTMLButtonElement;

    private board: Board;
    private bus: EventBus;

    constructor(addr: string) {

        this.connBtn = document.getElementById("btConn") as HTMLButtonElement;
        
        document.getElementById("btConn").onclick =  (e: Event) => {
            this.bus = new EventBus();

            if (this.connected == false) {
                this.ws = new WebSocket(addr);
                this.connBtn.value = "Disconnect";
                this.connected = true;
            } else {
                this.ws.close();
            }
        

            this.ws.onopen = (e: Event) => {
                this.bus.Subscribe("btn", (upd) => {this.sendGPIO(upd)});
                this.board = new Board(this.bus);
            }


            this.ws.onmessage = (e: MessageEvent) => {
                const upd : BoardUpdates = JSON.parse(e.data);
                console.log(upd);
                this.wsMsgDispatcher(upd);
            }

            this.ws.onclose = (e: Event) => {
                this.connBtn.value = "Connect to board";
                this.connected = false;
            }

        };
    }


    private sendGPIO(data : GPIO) {
        this.ws.send(JSON.stringify({gpio : data}));
    }


    private wsMsgDispatcher(upds : BoardUpdates) {
    
        if (upds.gpio) {
            upds.gpio.forEach((upd: GPIO) => {
                this.bus.Publish("led", upd);
            })
        }
        if (upds.sevseg) {
            this.bus.Publish("sevseg", upds);
        }
    }
}

let cl = new NexysClient(`ws://${window.location.hostname}:8081`);
