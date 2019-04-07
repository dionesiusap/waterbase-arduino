#include <ros.h>
#include <arduino_msgs/EffectorsMsg.h>

void cmdCb(const arduino_msgs::EffectorsMsg& msg);

ros::NodeHandle nh;

ros::Subscriber<arduino_msgs::EffectorsMsg> cmdSub("/cmd/eff", &cmdCb);

void setup() {
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT); // PWM pin

  nh.getHardware()->setBaud(9600);
  nh.initNode();
  nh.subscribe(cmdSub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}

void cmdCb(const arduino_msgs::EffectorsMsg& msg) {
  if (msg.light1) {
    digitalWrite(8, HIGH);
  }
  else {
    digitalWrite(8, LOW);
  }

  if (msg.light2) {
    digitalWrite(12, HIGH);
  }
  else {
    digitalWrite(12, LOW);
  }

  if (msg.light3) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }

  analogWrite(9, msg.ac);
}
