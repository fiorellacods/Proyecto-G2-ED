
#include "mail.h"

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
#include <LittleFS.h>

#define WIFI_SSID "UA-Alumnos"
#define WIFI_PASSWORD "41umn05WLC"

/** este es el host de el mail que usamos (el del proyectoed)*/
#define SMTP_HOST "smtp.office365.com"

/** The smtp port e.g. 
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
*/
#define SMTP_PORT 587

/* The log in credentials */
#define AUTHOR_EMAIL "proyectoEDG2@outlook.com"
#define AUTHOR_PASSWORD "Aprobamos"

/* Recipient's email*/
#define RECIPIENT_EMAIL "candelemos02@gmail.com"
/* The SMTP Session object used for Email sending */

SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

int sendMail(void){
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();




#if 0
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS mounted successfully");
  }
#endif
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "mydomain.net";

  /* Declare the message class */
  SMTP_Message message;

  /* Enable the chunked data transfer with pipelining for large message if server supported */
  message.enable.chunking = true;

  /* Set the message headers */
  message.sender.name = "ESP Mail";
  message.sender.email = AUTHOR_EMAIL;
  Serial.println("1 - llegue hasta la imagen");
  message.subject = "Test sending Email with attachments and inline images from SD card and Flash";
  message.addRecipient("Estimado dueño de la casa:", RECIPIENT_EMAIL);

  /** Two alternative content versions are sending in this example e.g. plain text and html */
  /** el link de la foto lo tenemos que poner entre barras, antes de cada "hay que usar \ menos adelante y atras"*/
  String htmlMsg = "<p>\"Hay un intruso en tu casa, adjunto la foto.\"</p><img src=\"\https://lh3.googleusercontent.com/u/2/drive-viewer/AFGJ81pk6QWQrHOXl3Bmlau5PcByzFzs0kfoQZsBKG0JeJ1iGodr_KPmVyzaZUgou-aFYjLLJP5bxGkoRJt52XkYp3zwqOM1=w3360-h1936\" class=\"ndfHFb-c4YZDc-HiaYvf-RJLb9c\" alt=\"Displaying WhatsApp Image 2023-06-09 at 13.52.51.jpeg\" aria-hidden=\"true\">";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

#if 0
  /* The attachment data item */
  SMTP_Attachment att;

  /** Set the attachment info e.g. 
   * file name, MIME type, file path, file storage type,
   * transfer encoding and content encoding
  */
  att.descr.filename = "image.png";
  att.descr.mime = "image/png"; //binary data
  att.file.path = "/data/image.png";
  att.file.storage_type = esp_mail_file_storage_type_flash;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  Serial.println("llegue hasta la imagen");

  /* Add attachment to the message */
  message.addAttachment(att);
#endif 
  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return 0;

  /* Start sending the Email and close the session */
  MailClient.sendMail(&smtp, &message, true);
  return 1;
}


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
}