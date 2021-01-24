using System;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;

namespace Almostengr.Tuscblackchamber.Tests
{
    public class BaseTest
    {
        public IWebDriver StartBrowser()
        {
            IWebDriver driver = null;
            ChromeOptions options = new ChromeOptions();

#if RELEASE
            // options.AddArgument("--headless");
#endif

            driver = new ChromeDriver(options);
            driver.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(15);

            return driver;
        }

        public void GoHome(IWebDriver driver)
        {
#if RELEASE
            driver.Navigate().GoToUrl("http://tuscblackchamber.org");
#else
            driver.Navigate().GoToUrl("http://192.168.57.117:8080");
#endif
        }

        public void CloseBrowser(IWebDriver driver)
        {
            if (driver != null)
            {
                driver.Quit();
            }
        }
    }
}