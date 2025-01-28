#include <EEPROM.h>
#include <LiquidCrystal_Menu_Btns.h>
#include <FlowSensor.h>

#define MeasureCoef 100

#define type 2280
#define pin 3
#define buzzer 12

FlowSensor Sensor(type, pin);
unsigned long timebefore = 0;  // Same type as millis()
unsigned long reset = 0;

void count() {
  Sensor.count();
}

LiquidCrystal_Menu_Btns lcdDisplay(16, 2);

// Параметры, которые мы будем именять
uint32_t _total = 0;
char *_min_Name = "";
int _min_val = 1000000;

char *_filterName1 = "Filter1";
uint16_t _lifetime1 = 10000;
uint16_t _usedLifetime1 = 0;
bool _active1 = true;

char *_filterName2 = "Filter2";
uint16_t _lifetime2 = 20000;
uint16_t _usedLifetime2 = 0;
bool _active2 = true;

char *_filterName3 = "Filter3";
uint16_t _lifetime3 = 10000;
uint16_t _usedLifetime3 = 0;
bool _active3 = true;

char *_filterName4 = "Filter4";
uint16_t _lifetime4 = 5000;
uint16_t _usedLifetime4 = 0;
bool _active4 = true;

char *_filterName5 = "Filter5";
uint16_t _lifetime5 = 5000;
uint16_t _usedLifetime5 = 0;
bool _active5 = true;

char *_filterName6 = "Filter6";
uint16_t _lifetime6 = 1000;
uint16_t _usedLifetime6 = 0;
bool _active6 = false;

enum {

  _addr_filterName1 = 0,
  _addr_lifetime1 = _addr_filterName1 + 16,
  _addr_usedLifetime1 = _addr_lifetime1 + sizeof(_lifetime1),
  _addr_active1 = _addr_usedLifetime1 + sizeof(_usedLifetime1),

  _addr_filterName2 = _addr_active1 + sizeof(_active1),
  _addr_lifetime2 = _addr_filterName2 + 16,
  _addr_usedLifetime2 = _addr_lifetime2 + sizeof(_lifetime2),
  _addr_active2 = _addr_usedLifetime2 + sizeof(_usedLifetime2),

  _addr_filterName3 = _addr_active2 + sizeof(_active2),
  _addr_lifetime3 = _addr_filterName3 + 16,
  _addr_usedLifetime3 = _addr_lifetime3 + sizeof(_lifetime3),
  _addr_active3 = _addr_usedLifetime3 + sizeof(_usedLifetime3),

  _addr_filterName4 = _addr_active3 + sizeof(_active3),
  _addr_lifetime4 = _addr_filterName4 + 16,
  _addr_usedLifetime4 = _addr_lifetime4 + sizeof(_lifetime4),
  _addr_active4 = _addr_usedLifetime4 + sizeof(_usedLifetime4),

  _addr_filterName5 = _addr_active4 + sizeof(_active4),
  _addr_lifetime5 = _addr_filterName5 + 16,
  _addr_usedLifetime5 = _addr_lifetime5 + sizeof(_lifetime5),
  _addr_active5 = _addr_usedLifetime5 + sizeof(_usedLifetime5),

  _addr_filterName6 = _addr_active5 + sizeof(_active5),
  _addr_lifetime6 = _addr_filterName6 + 16,
  _addr_usedLifetime6 = _addr_lifetime6 + sizeof(_lifetime6),
  _addr_active6 = _addr_usedLifetime6 + sizeof(_usedLifetime6),

  _addr_total = _addr_active6 + sizeof(_active6),
};

// Объявим перечисление, используемое в качестве ключа пунктов меню
enum { mkDashScreen,
       mkRoot,
       mkFilter1,
       mkFilter2,
       mkFilter3,
       mkFilter4,
       mkFilter5,
       mkFilter6,
       mkActive1,
       mkLifetime1,
       mkRename1,
       mkReset1,
       mkActive2,
       mkLifetime2,
       mkRename2,
       mkReset2,
       mkActive3,
       mkLifetime3,
       mkRename3,
       mkReset3,
       mkActive4,
       mkLifetime4,
       mkRename4,
       mkReset4,
       mkActive5,
       mkLifetime5,
       mkRename5,
       mkReset5,
       mkActive6,
       mkLifetime6,
       mkRename6,
       mkReset6,
       mkFactoryReset };

// Прототипы обработчиков пунктов меню:
void SetFilterName1();
void SetLifetime1();
void SetActive1();
void SetReset1();

void SetFilterName2();
void SetLifetime2();
void SetActive2();
void SetReset2();

void SetFilterName3();
void SetLifetime3();
void SetActive3();
void SetReset3();

void SetFilterName4();
void SetLifetime4();
void SetActive4();
void SetReset4();

void SetFilterName5();
void SetLifetime5();
void SetActive5();
void SetReset5();

void SetFilterName6();
void SetLifetime6();
void SetActive6();
void SetReset6();

void SetFactoryReset();

void updateMin();

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menu[] = {
  { mkDashScreen, mkRoot, "Flow meter" },
  { mkRoot, mkFilter1, NULL },
  { mkFilter1, mkActive1, NULL, SetActive1 },
  { mkFilter1, mkLifetime1, NULL, SetLifetime1 },
  { mkFilter1, mkRename1, "Rename", SetFilterName1 },
  { mkFilter1, mkReset1, "Reset", SetReset1 },
  { mkRoot, mkFilter2, NULL },
  { mkFilter2, mkActive2, NULL, SetActive2 },
  { mkFilter2, mkLifetime2, NULL, SetLifetime2 },
  { mkFilter2, mkRename2, "Rename", SetFilterName2 },
  { mkFilter2, mkReset2, "Reset", SetReset2 },
  { mkRoot, mkFilter3, NULL },
  { mkFilter3, mkActive3, NULL, SetActive3 },
  { mkFilter3, mkLifetime3, NULL, SetLifetime3 },
  { mkFilter3, mkRename3, "Rename", SetFilterName3 },
  { mkFilter3, mkReset3, "Reset", SetReset3 },
  { mkRoot, mkFilter4, NULL },
  { mkFilter4, mkActive4, NULL, SetActive4 },
  { mkFilter4, mkLifetime4, NULL, SetLifetime4 },
  { mkFilter4, mkRename4, "Rename", SetFilterName4 },
  { mkFilter4, mkReset4, "Reset", SetReset4 },
  { mkRoot, mkFilter5, NULL },
  { mkFilter5, mkActive5, NULL, SetActive5 },
  { mkFilter5, mkLifetime5, NULL, SetLifetime5 },
  { mkFilter5, mkRename5, "Rename", SetFilterName5 },
  { mkFilter5, mkReset5, "Reset", SetReset5 },
  { mkRoot, mkFilter6, NULL },
  { mkFilter6, mkActive6, NULL, SetActive6 },
  { mkFilter6, mkLifetime6, NULL, SetLifetime6 },
  { mkFilter6, mkRename6, "Rename", SetFilterName6 },
  { mkFilter6, mkReset6, "Reset", SetReset6 },
  { mkRoot, mkFactoryReset, "Factory Reset", SetFactoryReset },
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// Функция поиска индекса пункта по его ключу
int getItemIndexByKey(uint8_t key) {
  for (uint8_t i = 0; i < menuLen; i++)
    if (menu[i].key == key)
      return i;
  return -1;
}

// Функция формирования названия пункта меню, содержащего значение параметра
void updateCaption(uint8_t key, char format[], int value) {
  // key - ключ пункта меню, для которого обновляется навание
  // format - шаблон названия со значением
  // value - значение, добавляемое в название
  uint8_t index = getItemIndexByKey(key);
  char *buf = (char *)malloc(40);
  sprintf(buf, format, value);
  menu[index].caption = (char *)realloc(menu[index].caption, strlen(buf) + 1);
  strcpy(menu[index].caption, buf);
  free(buf);
}

void SetFilterName1() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName1, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName1, _filterName1);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName1 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter1, buf, _lifetime1 - (_usedLifetime1 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime1() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime1, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime1, _lifetime1);
    // Обновляем название пункта меню
    updateCaption(mkLifetime1, "Lifetime %d", _lifetime1);
    updateMin();
  }
}

void SetActive1() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active1 = lcdDisplay.selectVal("Active ?", list, 2, true, _active1 ? 0 : 1) == 0;
  if (_active1) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active1, _active1 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive1, "Active %s", _active1 ? "Yes" : "No");
  updateMin();
}

void SetReset1() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime1 = 0;
    EEPROM.update(0, _usedLifetime1);
    char buf[40];
    (_filterName1 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter1, buf, _lifetime1 - _usedLifetime1 / MeasureCoef);
    updateMin();
  }
}

void SetFilterName2() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName2, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName2, _filterName2);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName2 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter2, buf, _lifetime2 - (_usedLifetime2 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime2() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime2, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime2, _lifetime2);
    // Обновляем название пункта меню
    updateCaption(mkLifetime2, "Lifetime %d", _lifetime2);
    updateMin();
  }
}

void SetActive2() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active2 = lcdDisplay.selectVal("Active ?", list, 2, true, _active2 ? 0 : 1) == 0;
  if (_active2) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active2, _active2 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive2, "Active %s", _active2 ? "Yes" : "No");
  updateMin();
}

void SetReset2() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime2 = 0;
    EEPROM.update(0, _usedLifetime2);
    char buf[40];
    (_filterName2 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter2, buf, _lifetime2 - _usedLifetime2 / MeasureCoef);
    updateMin();
  }
}

void SetFilterName3() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName3, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName3, _filterName3);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName3 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter3, buf, _lifetime3 - (_usedLifetime3 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime3() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime3, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime3, _lifetime3);
    // Обновляем название пункта меню
    updateCaption(mkLifetime3, "Lifetime %d", _lifetime3);
    updateMin();
  }
}

void SetActive3() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active3 = lcdDisplay.selectVal("Active ?", list, 2, true, _active3 ? 0 : 1) == 0;
  if (_active3) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active3, _active3 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive3, "Active %s", _active3 ? "Yes" : "No");
  updateMin();
}

void SetReset3() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime3 = 0;
    EEPROM.update(0, _usedLifetime3);
    char buf[40];
    (_filterName3 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter3, buf, _lifetime3 - _usedLifetime3 / MeasureCoef);
    updateMin();
  }
}

void SetFilterName4() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName4, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName4, _filterName4);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName4 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter4, buf, _lifetime4 - (_usedLifetime4 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime4() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime4, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime4, _lifetime4);
    // Обновляем название пункта меню
    updateCaption(mkLifetime4, "Lifetime %d", _lifetime4);
    updateMin();
  }
}

void SetActive4() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active4 = lcdDisplay.selectVal("Active ?", list, 2, true, _active4 ? 0 : 1) == 0;
  if (_active4) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active4, _active4 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive4, "Active %s", _active4 ? "Yes" : "No");
  updateMin();
}

void SetReset4() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime4 = 0;
    EEPROM.update(0, _usedLifetime4);
    char buf[40];
    (_filterName4 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter4, buf, _lifetime4 - _usedLifetime4 / MeasureCoef);
    updateMin();
  }
}

void SetFilterName5() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName5, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName5, _filterName5);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName5 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter5, buf, _lifetime5 - (_usedLifetime5 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime5() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime5, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime5, _lifetime5);
    // Обновляем название пункта меню
    updateCaption(mkLifetime5, "Lifetime %d", _lifetime5);
    updateMin();
  }
}

void SetActive5() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active5 = lcdDisplay.selectVal("Active ?", list, 2, true, _active5 ? 0 : 1) == 0;
  if (_active5) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active5, _active5 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive5, "Active %s", _active5 ? "Yes" : "No");
  updateMin();
}

void SetReset5() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime5 = 0;
    EEPROM.update(0, _usedLifetime5);
    char buf[40];
    (_filterName5 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter5, buf, _lifetime5 - _usedLifetime5 / MeasureCoef);
    updateMin();
  }
}

void SetFilterName6() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputStrVal("Input caption", _filterName6, 13, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789 ")) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_filterName6, _filterName6);
    // Обновляем название пункта меню
    char buf[40];
    (_filterName6 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter6, buf, _lifetime6 - (_usedLifetime6 + (uint16_t)(Sensor.getVolume() * MeasureCoef) / MeasureCoef));
    updateMin();
  }
}

void SetLifetime6() {
  // Запрашиваем новое значение
  if (lcdDisplay.inputValBitwise("Filter lifetime", _lifetime6, 6, 0)) {
    // Сохраняем его в EEPROM
    EEPROM.update(_addr_lifetime6, _lifetime6);
    // Обновляем название пункта меню
    updateCaption(mkLifetime6, "Lifetime %d", _lifetime6);
    updateMin();
  }
}

void SetActive6() {
  // Запрашиваем новое значение
  String list[] = { "Yes", "No" };
  _active6 = lcdDisplay.selectVal("Active ?", list, 2, true, _active6 ? 0 : 1) == 0;
  if (_active6) {
    lcdDisplay.print("Filter enabled");
  } else {
    lcdDisplay.print("Filter disabled");
  }
  delay(1000);
  // Сохраняем его в EEPROM
  EEPROM.update(_addr_active6, _active6 ? 1 : 0);
  // Обновляем название пункта меню
  updateCaption(mkActive6, "Active %s", _active6 ? "Yes" : "No");
  updateMin();
}

void SetReset6() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Reset amount ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    _usedLifetime6 = 0;
    EEPROM.update(_addr_usedLifetime6, _usedLifetime6);
    char buf[40];
    (_filterName6 + String(" %d")).toCharArray(buf, (uint8_t)40);
    updateCaption(mkFilter6, buf, _lifetime6 - _usedLifetime6 / MeasureCoef);
    updateMin();
  }
}

void SetFactoryReset() {
  String list[] = { "No", "Yes" };
  bool yes = lcdDisplay.selectVal("Factory reset ?", list, 2, true, 0) == 1;
  if (yes) {
    lcdDisplay.print("Reset success");
  } else {
    lcdDisplay.print("Reset calceled");
  }
  delay(1000);
  if (yes) {
    EEPROM.update(_addr_total, 255);

    EEPROM.update(_addr_filterName1, 255);
    EEPROM.update(_addr_lifetime1, 255);
    EEPROM.update(_addr_usedLifetime1, 255);
    EEPROM.update(_addr_active1, 255);

    EEPROM.update(_addr_filterName2, 255);
    EEPROM.update(_addr_lifetime2, 255);
    EEPROM.update(_addr_usedLifetime2, 255);
    EEPROM.update(_addr_active2, 255);

    EEPROM.update(_addr_filterName3, 255);
    EEPROM.update(_addr_lifetime3, 255);
    EEPROM.update(_addr_usedLifetime3, 255);
    EEPROM.update(_addr_active3, 255);

    EEPROM.update(_addr_filterName4, 255);
    EEPROM.update(_addr_lifetime4, 255);
    EEPROM.update(_addr_usedLifetime4, 255);
    EEPROM.update(_addr_active4, 255);

    EEPROM.update(_addr_filterName5, 255);
    EEPROM.update(_addr_lifetime5, 255);
    EEPROM.update(_addr_usedLifetime5, 255);
    EEPROM.update(_addr_active5, 255);

    EEPROM.update(_addr_filterName6, 255);
    EEPROM.update(_addr_lifetime6, 255);
    EEPROM.update(_addr_usedLifetime6, 255);
    EEPROM.update(_addr_active6, 255);

    _total = 0;
    _min_Name = "";
    _min_val = 1000000;

    _filterName1 = "Filter1";
    _lifetime1 = 10000;
    _usedLifetime1 = 0;
    _active1 = true;

    _filterName2 = "Filter2";
    _lifetime2 = 20000;
    _usedLifetime2 = 0;
    _active2 = true;

    _filterName3 = "Filter3";
    _lifetime3 = 10000;
    _usedLifetime3 = 0;
    _active3 = true;

    _filterName4 = "Filter4";
    _lifetime4 = 5000;
    _usedLifetime4 = 0;
    _active4 = true;

    _filterName5 = "Filter5";
    _lifetime5 = 5000;
    _usedLifetime5 = 0;
    _active5 = true;

    _filterName6 = "Filter6";
    _lifetime6 = 1000;
    _usedLifetime6 = 0;
    _active6 = false;

    Sensor.resetVolume();
    reset = 0;
  }
}

void idleFunc() {
  if (millis() - timebefore >= 1000) {
    Sensor.read();
    if (Sensor.getFlowRate_m() > 0) {
      lcdDisplay.backlight();
      updateMin();
      lcdDisplay.printfAt(0, 0, "%8s%8d", _min_Name, _min_val);
      lcdDisplay.printfAt(0, 1, "%8s%8s", "Total", String((_total + Sensor.getVolume() * MeasureCoef) / MeasureCoef).c_str());
      if (_min_val < 100) {
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
      }
      if (_min_val < 50) {
        digitalWrite(buzzer, LOW);
        delay(1000);
        digitalWrite(buzzer, HIGH);
      }
      if (_min_val < 10) {
        digitalWrite(buzzer, LOW);
      }
      timebefore = millis();
    } else {
      digitalWrite(buzzer, HIGH);
    }
  }

  // Reset Volume
  if (millis() - reset >= 60000) {
    lcdDisplay.noBacklight();
    EEPROM_uint32_write(_addr_total, _total += (uint32_t)(Sensor.getVolume() * MeasureCoef));
    if (_active1) {
      EEPROM_uint16_write(_addr_usedLifetime1, _usedLifetime1 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName1 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter1, buf, _lifetime1 - _usedLifetime1 / MeasureCoef);
    }
    if (_active2) {
      EEPROM_uint16_write(_addr_usedLifetime2, _usedLifetime2 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName2 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter2, buf, _lifetime2 - _usedLifetime2 / MeasureCoef);
    }
    if (_active3) {
      EEPROM_uint16_write(_addr_usedLifetime3, _usedLifetime3 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName3 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter3, buf, _lifetime3 - _usedLifetime3 / MeasureCoef);
    }
    if (_active4) {
      EEPROM_uint16_write(_addr_usedLifetime4, _usedLifetime4 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName4 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter4, buf, _lifetime4 - _usedLifetime4 / MeasureCoef);
    }
    if (_active5) {
      EEPROM_uint16_write(_addr_usedLifetime5, _usedLifetime5 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName5 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter5, buf, _lifetime5 - _usedLifetime5 / MeasureCoef);
    }
    if (_active6) {
      EEPROM_uint16_write(_addr_usedLifetime6, _usedLifetime6 += (uint16_t)(Sensor.getVolume() * MeasureCoef));
      char buf[40];
      (_filterName6 + String(" %d")).toCharArray(buf, (uint8_t)40);
      updateCaption(mkFilter6, buf, _lifetime6 - _usedLifetime6 / MeasureCoef);
    }
    updateMin();
    Sensor.resetVolume();
    lcdDisplay.printfAt(0, 0, "%8s%8d", _min_Name, _min_val);
    lcdDisplay.printfAt(0, 1, "%8s%8s", "Total", String((_total + Sensor.getVolume() * MeasureCoef) / MeasureCoef).c_str());
    reset = millis();
  }
}

void setup() {
  Sensor.begin(count);

  lcdDisplay.begin();

  lcdDisplay.attachIdleFunc(idleFunc);

  // Считываем значения из EEPROM
  _total = EEPROM_uint32_read(_addr_total, _total);

  _active1 = EEPROM_uint8_read(_addr_active1, _active1) == 1;
  _filterName1 = EEPROM_string_read(_addr_filterName1, _filterName1);
  _lifetime1 = EEPROM_uint16_read(_addr_lifetime1, _lifetime1);
  _usedLifetime1 = EEPROM_uint16_read(_addr_usedLifetime1, _usedLifetime1);

  _active2 = EEPROM_uint8_read(_addr_active2, _active2) == 1;
  _filterName2 = EEPROM_string_read(_addr_filterName2, _filterName2);
  _lifetime2 = EEPROM_uint16_read(_addr_lifetime2, _lifetime2);
  _usedLifetime2 = EEPROM_uint16_read(_addr_usedLifetime2, _usedLifetime2);

  _active3 = EEPROM_uint8_read(_addr_active3, _active3) == 1;
  _filterName3 = EEPROM_string_read(_addr_filterName3, _filterName3);
  _lifetime3 = EEPROM_uint16_read(_addr_lifetime3, _lifetime3);
  _usedLifetime3 = EEPROM_uint16_read(_addr_usedLifetime3, _usedLifetime3);

  _active4 = EEPROM_uint8_read(_addr_active4, _active4) == 1;
  _filterName4 = EEPROM_string_read(_addr_filterName4, _filterName4);
  _lifetime4 = EEPROM_uint16_read(_addr_lifetime4, _lifetime4);
  _usedLifetime4 = EEPROM_uint16_read(_addr_usedLifetime4, _usedLifetime4);

  _active5 = EEPROM_uint8_read(_addr_active5, _active5) == 1;
  _filterName5 = EEPROM_string_read(_addr_filterName5, _filterName5);
  _lifetime5 = EEPROM_uint16_read(_addr_lifetime5, _lifetime5);
  _usedLifetime5 = EEPROM_uint16_read(_addr_usedLifetime5, _usedLifetime5);

  _active6 = EEPROM_uint8_read(_addr_active6, _active6) == 1;
  _filterName6 = EEPROM_string_read(_addr_filterName6, _filterName6);
  _lifetime6 = EEPROM_uint16_read(_addr_lifetime6, _lifetime6);
  _usedLifetime6 = EEPROM_uint16_read(_addr_usedLifetime6, _usedLifetime6);

  // Формируем названия пунктов меню
  updateCaption(mkActive1, "Active %s", _active1 ? "Yes" : "No");
  char buf1[40];
  (_filterName1 + String(" %d")).toCharArray(buf1, (uint8_t)40);
  updateCaption(mkFilter1, buf1, _lifetime1 - _usedLifetime1 / MeasureCoef);
  updateCaption(mkLifetime1, "Lifetime %d", _lifetime1);

  updateCaption(mkActive2, "Active %s", _active2 ? "Yes" : "No");
  char buf2[40];
  (_filterName2 + String(" %d")).toCharArray(buf2, (uint8_t)40);
  updateCaption(mkFilter2, buf2, _lifetime2 - _usedLifetime2 / MeasureCoef);
  updateCaption(mkLifetime2, "Lifetime %d", _lifetime2);

  updateCaption(mkActive3, "Active %s", _active3 ? "Yes" : "No");
  char buf3[40];
  (_filterName3 + String(" %d")).toCharArray(buf3, (uint8_t)40);
  updateCaption(mkFilter3, buf3, _lifetime3 - _usedLifetime3 / MeasureCoef);
  updateCaption(mkLifetime3, "Lifetime %d", _lifetime3);

  updateCaption(mkActive4, "Active %s", _active4 ? "Yes" : "No");
  char buf4[40];
  (_filterName4 + String(" %d")).toCharArray(buf4, (uint8_t)40);
  updateCaption(mkFilter4, buf4, _lifetime4 - _usedLifetime4 / MeasureCoef);
  updateCaption(mkLifetime4, "Lifetime %d", _lifetime4);

  updateCaption(mkActive5, "Active %s", _active5 ? "Yes" : "No");
  char buf5[40];
  (_filterName5 + String(" %d")).toCharArray(buf5, (uint8_t)40);
  updateCaption(mkFilter5, buf5, _lifetime5 - _usedLifetime5 / MeasureCoef);
  updateCaption(mkLifetime5, "Lifetime %d", _lifetime5);

  updateCaption(mkActive6, "Active %s", _active6 ? "Yes" : "No");
  char buf6[40];
  (_filterName6 + String(" %d")).toCharArray(buf6, (uint8_t)40);
  updateCaption(mkFilter6, buf6, _lifetime6 - _usedLifetime6 / MeasureCoef);
  updateCaption(mkLifetime6, "Lifetime %d", _lifetime6);

  lcdDisplay.backlight();
  Sensor.read();
  updateMin();
  lcdDisplay.printfAt(0, 0, "%8s%8d", _min_Name, _min_val);
  lcdDisplay.printfAt(0, 1, "%8s%8s", "Total", String((_total + Sensor.getVolume() * MeasureCoef) / MeasureCoef).c_str());

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(buzzer, HIGH);
}

void updateMin() {
  if (_min_val > _lifetime1 - (_usedLifetime1 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active1) {
    _min_Name = _filterName1;
    _min_val = _lifetime1 - (_usedLifetime1 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
  if (_min_val > _lifetime2 - (_usedLifetime2 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active2) {
    _min_Name = _filterName2;
    _min_val = _lifetime2 - (_usedLifetime2 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
  if (_min_val > _lifetime3 - (_usedLifetime3 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active3) {
    _min_Name = _filterName3;
    _min_val = _lifetime3 - (_usedLifetime3 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
  if (_min_val > _lifetime4 - (_usedLifetime4 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active4) {
    _min_Name = _filterName4;
    _min_val = _lifetime4 - (_usedLifetime4 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
  if (_min_val > _lifetime5 - (_usedLifetime5 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active5) {
    _min_Name = _filterName5;
    _min_val = _lifetime5 - (_usedLifetime5 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
  if (_min_val > _lifetime6 - (_usedLifetime6 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef && _active6) {
    _min_Name = _filterName6;
    _min_val = _lifetime6 - (_usedLifetime6 + (uint32_t)(Sensor.getVolume() * MeasureCoef)) / MeasureCoef;
  }
}

void loop() {
  if (lcdDisplay.getButtonsState() == eNone) {
    idleFunc();
  } else {
    lcdDisplay.backlight();
    // Показываем меню
    uint8_t selectedMenuItem = lcdDisplay.showMenu(menu, menuLen, 1);

    if (selectedMenuItem == mkDashScreen) {
      Sensor.read();
      updateMin();
      lcdDisplay.printfAt(0, 0, "%8s%8d", _min_Name, _min_val);
      lcdDisplay.printfAt(0, 1, "%8s%8s", "Total", String((_total + Sensor.getVolume() * MeasureCoef) / MeasureCoef).c_str());
    }
  }
}

int EEPROM_string_write(int addrOffset, String strToWrite) {
  strToWrite = strToWrite.substring(0, 15);
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++) {
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}

char *EEPROM_string_read(int addrOffset, char *defaultValue) {
  int newStrLen = EEPROM.read(addrOffset);
  if (newStrLen == 255) {
    return defaultValue;
  }
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  return data;
}

uint16_t EEPROM_uint16_read(int addr, uint16_t defaultValue) {
  uint8_t check = EEPROM.read(addr);
  if (check == 255) {
    return defaultValue;
  }
  byte raw[2];
  for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr + i);
  uint16_t &num = (uint16_t &)raw;
  return num;
}

void EEPROM_uint16_write(int addr, uint16_t num) {
  byte raw[2];
  (uint16_t &)raw = num;
  for (byte i = 0; i < 2; i++) EEPROM.update(addr + i, raw[i]);
}

uint8_t EEPROM_uint8_read(int addr, uint8_t defaultValue) {
  uint8_t val = EEPROM.read(addr);
  if (val == 255) {
    return defaultValue;
  } else {
    return val;
  }
}

void EEPROM_uint8_write(int addr, uint8_t num) {
  EEPROM.update(addr, num);
}

uint32_t EEPROM_uint32_read(int addr, uint32_t defaultValue) {
  uint8_t check = EEPROM.read(addr);
  if (check == 255) {
    return defaultValue;
  }
  byte raw[4];
  for (byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr + i);
  uint32_t &num = (uint32_t &)raw;
  return num;
}

void EEPROM_uint32_write(int addr, uint32_t num) {
  byte raw[4];
  (uint32_t &)raw = num;
  for (byte i = 0; i < 4; i++) EEPROM.update(addr + i, raw[i]);
}
