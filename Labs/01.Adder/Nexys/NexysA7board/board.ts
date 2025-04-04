
interface Toggler {
    Toggle() : void;
}


class Button implements Toggler {

    btn: SVGCircleElement;

    constructor(btn: HTMLElement) {
        this.btn = btn.querySelector("circle");

        btn.onclick = this.Toggle;
    }

    Toggle() : void {
        const r = this.btn.getAttribute('r')
        const is_on = r == "18";

        this.btn.setAttribute('r', is_on ? "10" : "18");
    }
}


class Switch extends Button {
    constructor(btn: HTMLElement) {
        super(btn)
    }

    Toggle() : void {
        const cy = this.btn.getAttribute('cy')
        const is_on = cy == "900";

        this.btn.setAttribute('cy', is_on ? "850" : "900");
    }
}

class LED {

    led: SVGCircleElement;

    constructor(led: SVGElement) {
        this.led = led.querySelector("circle");
    }

    Set(on: boolean) {
        this.led.setAttribute('opacity', on ? "1.0" : "0.1");
    }
}


export class Board {
    board: Document;

    buttons: Map<string, Button>;
    leds: Map<string, LED>;

    constructor() {
        let boardObject = document.getElementById('nexys-svg') as HTMLObjectElement;

        this.board = boardObject.contentDocument;

        this.buttons["SW1"] = new Switch(this.board.getElementById("SW1")); 
    }

    Update(data: Object) {

    }
}
