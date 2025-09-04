#include <IRremote.hpp>
#include <FastLED.h>

#define IR_RECEIVE_PIN 2
#define LED_PIN 11
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define ROWS 10
#define COLS 20

int head[2]; 
int body[COLS*ROWS][2];
int food[2];
int score;

bool finished = false;

int speed = 1000;         
byte brightness = 255;   

//     0
//  1     3
//     2
int snake_direction;

CRGB leds[ROWS * COLS];

// matrix
//      9     8     7     6     5     4     3     2     1     0
// 19 [199] [179] [159] [139] [119] [ 99] [ 79] [ 59] [ 39] [ 19]
// 18 [198] [178] [158] [138] [118] [ 98] [ 78] [ 58] [ 38] [ 18]
// 17 [197] [177] [157] [137] [117] [ 97] [ 77] [ 57] [ 37] [ 17]
// 16 [196] [176] [156] [136] [116] [ 96] [ 76] [ 56] [ 36] [ 16]
// 15 [195] [175] [155] [135] [115] [ 95] [ 75] [ 55] [ 35] [ 15]
// 14 [194] [174] [154] [134] [114] [ 94] [ 74] [ 54] [ 34] [ 14]
// 13 [193] [173] [153] [133] [113] [ 93] [ 73] [ 53] [ 33] [ 13]
// 12 [192] [172] [152] [132] [112] [ 92] [ 72] [ 52] [ 32] [ 12]
// 11 [191] [171] [151] [131] [111] [ 91] [ 71] [ 51] [ 31] [ 11]
// 10 [190] [170] [150] [130] [110] [ 90] [ 70] [ 50] [ 30] [ 10]
//  9 [189] [169] [149] [129] [109] [ 89] [ 69] [ 49] [ 29] [  9]
//  8 [188] [168] [158] [128] [108] [ 88] [ 68] [ 48] [ 28] [  8]
//  7 [187] [167] [147] [127] [107] [ 87] [ 67] [ 47] [ 27] [  7]
//  6 [186] [166] [146] [126] [106] [ 86] [ 66] [ 46] [ 26] [  6]
//  5 [185] [165] [145] [125] [105] [ 85] [ 65] [ 45] [ 25] [  5]
//  4 [184] [164] [144] [124] [104] [ 84] [ 64] [ 44] [ 24] [  4]
//  3 [183] [163] [143] [123] [103] [ 83] [ 63] [ 43] [ 23] [  3]
//  2 [182] [162] [142] [122] [102] [ 82] [ 62] [ 42] [ 22] [  2]
//  1 [181] [161] [141] [121] [101] [ 81] [ 61] [ 41] [ 21] [  1]
//  0 [180] [160] [140] [120] [100] [ 80] [ 60] [ 40] [ 20] [  0]

// actually soldered LEDs
//      9     8     7     6     5     4     3     2     1     0
// 19 [180] [179] [140] [139] [100] [ 99] [ 60] [ 59] [ 20] [ 19]
// 18 [181] [178] [141] [138] [101] [ 98] [ 61] [ 58] [ 21] [ 18]
// 17 [182] [177] [142] [137] [102] [ 97] [ 62] [ 57] [ 22] [ 17]
// 16 [183] [176] [143] [136] [103] [ 96] [ 63] [ 56] [ 23] [ 16]
// 15 [184] [175] [144] [135] [104] [ 95] [ 64] [ 55] [ 24] [ 15]
// 14 [185] [174] [145] [134] [105] [ 94] [ 65] [ 54] [ 25] [ 14]
// 13 [186] [173] [146] [133] [106] [ 93] [ 66] [ 53] [ 26] [ 13]
// 12 [187] [172] [147] [132] [107] [ 92] [ 67] [ 52] [ 27] [ 12]
// 11 [188] [171] [148] [131] [108] [ 91] [ 68] [ 51] [ 28] [ 11]
// 10 [180] [170] [149] [130] [109] [ 90] [ 69] [ 50] [ 29] [ 10]
//  9 [190] [169] [150] [129] [110] [ 89] [ 70] [ 49] [ 30] [  9]
//  8 [191] [168] [151] [128] [111] [ 88] [ 71] [ 48] [ 31] [  8]
//  7 [192] [167] [152] [127] [112] [ 87] [ 72] [ 47] [ 32] [  7]
//  6 [193] [166] [153] [126] [113] [ 86] [ 73] [ 46] [ 33] [  6]
//  5 [194] [165] [154] [125] [114] [ 85] [ 74] [ 45] [ 34] [  5]
//  4 [195] [164] [155] [124] [115] [ 84] [ 75] [ 44] [ 35] [  4]
//  3 [196] [163] [156] [123] [116] [ 83] [ 76] [ 43] [ 36] [  3]
//  2 [197] [162] [157] [122] [117] [ 82] [ 77] [ 42] [ 37] [  2]
//  1 [198] [161] [158] [121] [118] [ 81] [ 78] [ 41] [ 38] [  1]
//  0 [199] [160] [159] [120] [119] [ 80] [ 79] [ 40] [ 39] [  0]

void setup() 
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, COLS * ROWS);
    FastLED.clear();

    IrReceiver.begin(IR_RECEIVE_PIN, true);

    randomSeed(analogRead(0));

    reset_game();
}

void loop() 
{
    if (IrReceiver.decode()) {
        IrReceiver.resume();
        
        switch (IrReceiver.decodedIRData.command)
        {
            case 0x18:
            {
                if (snake_direction != 2)
                    snake_direction = 0;
                break;
            }
            case 0x5A:
            {
                if (snake_direction != 1)
                    snake_direction = 3;
                break;
            }
            case 0x52:
            {
                if (snake_direction != 0)
                    snake_direction = 2;
                break;
            }
            case 0x8:
            {
                if (snake_direction != 3)
                    snake_direction = 1;
                break;
            }
            case 0x40:
            {
                speed -= 100;
                if (speed < 100) 
                    speed = 100;
                break;
            }
            case 0x44:
            {
                speed += 10;
                if (speed > 2000) 
                    speed = 2000;
                break;
            }
            case 0x15:
            {
                brightness -= 25;
                if (brightness < 0) 
                    brightness = 0;
                break;
            }
            case 0x7:
            {
                brightness += 25;
                if (brightness > 255) 
                    brightness = 255;
                break;
            }
            case 0x47:
            {
                reset_game();
                break;
            }
            default:
                break;
        }
        
    }

    if (IrReceiver.isIdle() && !finished)
    {    
        move_snake();

        if (check_snake_eats_food())
        {
            score += 1;
            generate_food();
        }
        
        check_snake_hit_wall();
        check_snake_bit_itself();

        render();

        delay(speed);
    }
}


void render()
{
    FastLED.clear();
    FastLED.setBrightness(brightness);  

    leds[playfield_position_to_actual(food[0], food[1])] = CRGB::Red;

    for ( int i = 0; i <= score; i++)
    {
        leds[playfield_position_to_actual(body[i][0], body[i][1])] = CRGB::Green;
    }
    
    FastLED.show();
}

int playfield_position_to_actual(int col, int row)
{
    int is_odd_row = row % 2;
    int base = (row - is_odd_row) * COLS;

    if (is_odd_row == 0)
        return base + col;
    return base + (COLS * 2 - 1) - col;
}

void check_snake_hit_wall()
{
    if ((head[0] < 0) || (head[0] >= COLS))
        finished = true;
    if ((head[1] < 0) || (head[1] >= ROWS))
        finished = true;
}

void check_snake_bit_itself()
{
    for (int i = score; i>0; i--)
    {
        if ((head[0] == body[i][0]) && (head[1] == body[i][1]))
        {
            finished = true;
            break;
        }
    }
}

bool check_snake_eats_food()
{
    
    if ((head[0] == food[0]) && (head[1] == food[1]))
        return true;
    return false;
}

void move_snake()
{
    switch (snake_direction)
    {
        case 0:
        {
            head[0] += 1;
            break;
        }
        case 1:
        {
            head[1] += 1;
            break;
        }
        case 2:
        {
            head[0] -= 1;
            break;
        }
        case 3:
        {
            head[1] -= 1;
            break;
        }
    }

    for (int i = score; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
    body[0][0] = head[0];
    body[0][1] = head[1];
}

void generate_food()
{
    do
    {
        food[0] = random(0, COLS);
        food[1] = random(0, ROWS);
    } 
    while (check_body_intersects_food());
}

bool check_body_intersects_food()
{
    for (int i=score; i>=0; i--)
    {
        if ((food[0] == body[i][0]) && (food[1] == body[i][1]))
        {
            return true;
        }
    }
    return false;
}

void reset_game() 
{
    head[0] = COLS / 2;
    head[1] = ROWS / 2;

    body[0][0] = head[0];
    body[0][1] = head[1];

    food[0] = head[0] + 2;
    food[1] = head[1];
    
    score = 0;
    finished = false;
}