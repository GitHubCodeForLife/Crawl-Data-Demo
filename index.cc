const puppeteer = require("puppeteer");
var fs = require("fs");
let browser;
let data = [];

(async () => {
  browser = await puppeteer.launch({ headless: false, slowMo: 80 });
  // const browser = await puppeteer.launch({ headless: false, slowMo: 80 });
  const page = await browser.newPage();
  await page.setViewport({
    width: 1536,
    height: 900,
    timeout: 100000,
    deviceScaleFactor: 1,
  });
  await page.goto(
    "https://edu2review.com/danh-gia/truong-dai-hoc-kinh-te-tp-ho-chi-minh#courses-12",
    {
      waitUntil: "networkidle2",
    }
  );

  const viewMore = await page.$(".subtle-btn.subtle-btn__outlined.view-more");
  const readMoreTimes = 4;
  for (i = 0; i < readMoreTimes; i++) {
    await viewMore.click();
  }

  const read_more = await page.$$(".comment-block__content");
  for (let i = 0; i < read_more.length; i++) {
    let review = {};
    const current = read_more[i];
    //get all p tags
    const p = await current.$$("p");

    //get h3 tags
    const h3 = await current.$$("h3");
    //get h3 text
    review.overview = await h3[0].evaluate((node) => node.textContent);
    review.submit = await p[0].evaluate((node) => node.textContent);
    review.advantage = await p[2].evaluate((node) => node.textContent);
    review.disadvantage = await p[4].evaluate((node) => node.textContent);

    //exp and advise
    const read_more_trigger = await current.$$(".readmore-trigger");
    await read_more_trigger[0].click();
    review.exp = await p[6].evaluate((node) => node.textContent);
    data.push(review);
    console.log({ data });
  }
  //save to file json
  fs.writeFile("./data.json", JSON.stringify(data), (err) => {
    if (err) {
      console.log(err);
    }
  });

  await page.close();
})();
