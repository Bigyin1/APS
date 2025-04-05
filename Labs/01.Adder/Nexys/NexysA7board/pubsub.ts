
type Callback = (data: any) => void;

export class EventBus {

    private subscribers: Map<string, Callback[]> = new Map();

    Subscribe(eventType: string, callback: Callback): void {
        if (!this.subscribers.has(eventType)) {
            this.subscribers.set(eventType, []);
        }
        this.subscribers.get(eventType)!.push(callback);
    }

    Unsubscribe(eventType: string, callback: Callback): void {
        const callbacks = this.subscribers.get(eventType);
        if (callbacks) {
            this.subscribers.set(eventType, callbacks.filter(cb => cb !== callback));
        }
    }

    Publish(eventType: string, data: any): void {
        const callbacks = this.subscribers.get(eventType);
        if (callbacks) {
            callbacks.forEach(callback => callback(data));
        }
    }

}
