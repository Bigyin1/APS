import { Board } from "./board";


class NexysClient {
    addr: string;

    board: Board;

    constructor(addr: string) {
        this.addr = addr

        this.board = new Board();

        console.log("OK");
    }

}




let cl = new NexysClient(`http://${window.location.host}`);
