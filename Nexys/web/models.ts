export interface GPIO {
    id : string;
    on : boolean;
}

export type HexDigit = string & { length: 1 }

export interface SevSegArray {
    sevseg : [HexDigit, HexDigit, HexDigit, HexDigit, HexDigit, HexDigit, HexDigit];
}


export interface BoardUpdates {
    gpio? : GPIO[];
    sevseg? : SevSegArray;
}
