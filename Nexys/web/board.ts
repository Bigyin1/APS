import { EventBus } from "./pubsub.js";
import { GPIO, SevSegArray, HexDigit } from "./models.js";


abstract class Button {

    id: string;
    on = false;

    protected btn: SVGCircleElement;

    constructor(btn: HTMLElement, bus: EventBus, eventName: string) {
        this.btn = btn.querySelector("circle");
        this.id = btn.id;

        btn.onclick = (e) => {
            this.Toggle();

            bus.Publish(eventName, { on: this.on, id: this.id } as GPIO);
        };

        bus.Publish(eventName, { on: this.on, id: this.id } as GPIO) // initial state
    }

    protected abstract Toggle(): void;
}


class PushButton extends Button {

    private offR = "18";
    private onR = "11";

    constructor(btn: HTMLElement, bus: EventBus) {
        super(btn, bus, "btn")
    }

    protected Toggle(): void {
        this.on = !this.on;

        this.btn.setAttribute('r', this.on ? this.onR : this.offR);
    }
}


class Switch extends Button {

    private offY = "900";
    private onY = "850";

    constructor(btn: HTMLElement, bus: EventBus) {
        super(btn, bus, "btn")
    }

    protected Toggle(): void {
        this.on = !this.on;

        this.btn.setAttribute('cy', this.on ? this.onY : this.offY);
    }
}

class LED {
    id: string;

    private led: SVGCircleElement;

    constructor(led: HTMLElement, bus: EventBus) {
        this.led = led.querySelector("circle");
        this.id = led.id;

        bus.Subscribe("led", (upd) => { this.Set(upd) });
    }

    Set(upd: GPIO) {
        if (upd.id != this.id)
            return;

        this.led.setAttribute('opacity', upd.on ? "1.0" : "0.1");
    }
}


class SevSegDispl {
    private display: HTMLTextAreaElement[];

    constructor(display: HTMLElement, bus: EventBus) {
        this.display = Array.from(display.children) as HTMLTextAreaElement[]

        bus.Subscribe("sevseg", (upd) => { this.Set(upd) });
    }


    Set(upd: SevSegArray) {
        [...this.display].reverse().forEach((d: HTMLTextAreaElement, idx: number) => {
            d.value = upd.sevseg[idx];
            d.scrollTop = d.scrollHeight;
        })
    }
}


export class Board {
    board: Document;

    private buttons: Map<string, Button> = new Map();
    private leds: Map<string, LED> = new Map();
    private sevseg: SevSegDispl;

    constructor(bus: EventBus) {
        let boardObject = document.getElementById('nexys-svg') as HTMLObjectElement;
        let board = boardObject.contentDocument;

        for (let i = 0; i < 16; i++) {
            const sw_id = `SW${i}`;
            const sw_i = board.getElementById(sw_id);

            this.buttons[sw_id] = new Switch(sw_i, bus);
        }

        for (let i = 0; i < 16; i++) {
            const ld_id = `LD${i}`;
            const ld_i = board.getElementById(ld_id);

            this.leds[ld_id] = new LED(ld_i, bus);
        }

        this.buttons["BTNL"] = new PushButton(board.getElementById("BTNL"), bus)
        this.buttons["BTNC"] = new PushButton(board.getElementById("BTNC"), bus)
        this.buttons["BTNR"] = new PushButton(board.getElementById("BTNR"), bus)
        this.buttons["BTNU"] = new PushButton(board.getElementById("BTNU"), bus)
        this.buttons["BTND"] = new PushButton(board.getElementById("BTND"), bus)
        this.buttons["RSTN"] = new PushButton(board.getElementById("RSTN"), bus)

        this.sevseg = new SevSegDispl(document.getElementById("SevSegDispl"), bus);
    }
}
