using System;
using System.Threading;
using NUnit.Framework;
using OpenQA.Selenium;

namespace Almostengr.Tuscblackchamber.Tests
{
    public class ContactTests : BaseTest
    {
        private IWebDriver driver;

        [SetUp]
        public void Setup()
        {
            driver = StartBrowser();
        }

        [Test]
        public void ProcessValidContactSubmission()
        {
            GoHome(driver);

            driver.FindElement(By.LinkText("CONTACT")).Click();

            IWebElement customerFirst = driver.FindElement(By.Name("customerfirst"));
            customerFirst.SendKeys("Tuscaloosa");

            driver.FindElement(By.Name("customerlast")).SendKeys("Tester");
            driver.FindElement(By.Name("emailaddress")).SendKeys("tharam04@yahoo.com");
            driver.FindElement(By.Name("phonenumber")).SendKeys("205-555-1234");
            
            string repsonseMsg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam feugiat nunc et turpis bibendum tempus. Aliquam finibus euismod aliquam. Morbi semper lectus a mauris commodo egestas sit amet nec risus.";
            driver.FindElement(By.Name("cresponse")).SendKeys(repsonseMsg);

            customerFirst.Submit();

            // driver.FindElement(By.Id("successmessage"));

            Thread.Sleep(TimeSpan.FromSeconds(10));

            bool successMsg = driver.FindElement(By.Id("successmessage")).Displayed;
            bool failMsg = driver.FindElement(By.Id("failuremessage")).Displayed;

            // assert
            // Assert.Pass();
            Assert.IsTrue(successMsg);
            Assert.IsFalse(failMsg);

        }

        [OneTimeTearDown]
        public void TearDown()
        {
            CloseBrowser(driver);
        }
    }
}