import { Board } from "./board.js";


class NexysClient {
    addr: string;

    board: Board;
    

    constructor(addr: string) {
        this.addr = addr

        this.board = new Board();

        console.log("OK");
    }


}




let cl = new NexysClient(`ws://${window.location.host}:8081`);
