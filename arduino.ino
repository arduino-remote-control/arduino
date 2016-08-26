#include <IRremote.h>
#include <IRremoteInt.h>

#define ENTER 10
#define NET_TV 11

void setup();
void loop();
String *parse(String param, int *len);
void command(String *params, int len);

/****************************************/
decode_results result, last;
int bits = 32;
unsigned long cmd_number[12];
IRrecv irrecv(11);
IRsend sendIR;

/****************************************/

/****************************************/
void setup() {
    cmd_number[0] = 3782869247;
    cmd_number[1] = 3782901887;
    cmd_number[2] = 3782885567;
    cmd_number[3] = 3782918207;
    cmd_number[4] = 3782877407;
    cmd_number[5] = 3782910047;
    cmd_number[6] = 3782893727;
    cmd_number[7] = 3782926367;
    cmd_number[8] = 3782873327;
    cmd_number[9] = 3782905967;
    cmd_number[10] = 3782912087;
    cmd_number[11] = 3782879447;
    Serial.begin(9600);
    irrecv.enableIRIn();
}

void loop() {
    delay(50);
    if (Serial.available() >= 10) {
        int n = Serial.available();
        char *tmp = new char[n];
        for (int i = 0; i < n; i++) {
            tmp[i] = Serial.read();
        }
        tmp[n] = '\0';

        int l = 0;
        String *ret = parse(tmp, &l);
        command(ret, l);
        delete tmp, ret;
    }

    if (irrecv.decode(&result)) {
        if (result.value != 0xFFFFFFFF) {
            last = result;
            Serial.println(result.value);
        }
        irrecv.resume();
    }
}

String* parse(String param, int *len) {
    int a = 0;
    int f = 0;
    int i = 0;

    do {
        f = param.indexOf(';', f);
        if (f == -1)
            break;
        a = ++f;
        i++;
    } while (f != -1);

    ++i;
    String *ret = new String[i];
    *len = i;
    a = f = i = 0;

    do {
        f = param.indexOf(';', f);
        ret[i] = param.substring(a, f);
        ret[i].trim();
        if (f == -1)
            break;
        a = ++f;
        i++;
    } while (f != -1);

    return ret;
}

void command(String *params, int len) {
    String cmd = params[0];
    switch (cmd[0]) {
    case 'c':
        Serial.print(params[1]);
        for (int i = 0; i < params[1].length(); i++) {
            int pos = params[1][i]-48;
            sendIR.sendNEC(cmd_number[pos], 32);
            delay(300);
        }

        irrecv.enableIRIn();
        break;
    default:
        Serial.print("Command not found: ");
        Serial.println(cmd);
        break;
    }
}

