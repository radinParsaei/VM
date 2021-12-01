// Basically working source code that can be used to upload programs or to configure wifi through Serial

let port = await navigator.serial.requestPort();
await port.open({ baudRate: 9600 });
let decoder = new TextDecoderStream();
const textDecoder = new TextDecoderStream();
const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
const reader = textDecoder.readable.getReader();
const textEncoder = new TextEncoderStream();

const writableStreamClosed = textEncoder.readable.pipeTo(port.writable);

const writer = textEncoder.writable.getWriter();

setTimeout(function() {
    writer.write("\rWiFi stopAP"); // or any other command (see esp32-runtime.ino where commandLine() function developed)
}, 400)

setTimeout(async function() {
    reader.cancel();
    await readableStreamClosed.catch(() => {});

    writer.close();
    await writableStreamClosed;

    await port.close();
}, 1000)

while (true) {
    const { value, done } = await reader.read();
    if (value) {
        document.body.append(value);
    }
    if (done) {
        reader.releaseLock();
        break;
    }
}