#define W_SEND 'w'
#define W_RECV 'W'

struct Wifi {
    String name, pass;
} wifi;

void setup();
void loop();
String *parse(String param, int *len);
void command(String *params, int len);


/****************************************/
void setup() {
    Serial.begin(9600);
}

void loop() {
    delay(50);
    if (Serial.available()) {
        delay(50);
        int n = Serial.available();
        char *tmp = new char[n];
        for (int i = 0; i < n; i++) {
            tmp[i] = Serial.read();
        }
        tmp[n] = '\0';
        Serial.println(tmp);

        int l = 0;
        String *ret = parse(tmp, &l);
        command(ret, l);
        delete tmp, ret;
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
    case 'w':
        wifi.name = params[1];
        wifi.pass = params[2];
    case 'W':
        Serial.print("w;");
        Serial.print(wifi.name);
        Serial.print(";");
        Serial.println(wifi.pass);
        break;
    default:
        Serial.print("Command not found: ");
        Serial.println(cmd);
        break;
    }
}

