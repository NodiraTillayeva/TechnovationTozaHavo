function doPost(e) {
  try {
    var sheet = SpreadsheetApp.openById("1lE2kN_FOJWmcfoDzg48cIxxEQkBWmXW57xXvCE1-Wc4").getSheetByName("Sheet1");
    var data = JSON.parse(e.postData.contents);

    // Log the row with Google-generated timestamp
    sheet.appendRow([
      new Date(),
      data.lat,
      data.lon,
      data.aqi,
      data.mq4,
      data.mq7,
      data.mq8,
      data.co2
    ]);

    // Format the timestamp column (column 1)
    sheet.getRange(sheet.getLastRow(), 1).setNumberFormat("yyyy-mm-dd hh:mm:ss");

    return ContentService.createTextOutput("OK").setMimeType(ContentService.MimeType.TEXT);
  } catch (err) {
    return ContentService.createTextOutput("Error: " + err.message).setMimeType(ContentService.MimeType.TEXT);
  }
}
