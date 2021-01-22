using NUnit.Framework;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;

namespace Almostengr.Tuscblackchamber.Tests
{
    public class MembershipTests : BaseTest
    {
        private IWebDriver driver;

        [SetUp]
        public void Setup()
        {
            driver = StartBrowser();
        }

        [Test]
        public void ViewMembershipPage()
        {
            GoHome(driver);

            driver.FindElement(By.LinkText("MEMBERSHIP")).Click();

            string h1Tag = driver.FindElement(By.TagName("h1")).Text;

            bool memberDirectory = driver.PageSource.Contains("TABCC Membership Directory");

            Assert.IsTrue(memberDirectory);
            Assert.AreEqual("Membership", h1Tag);
        }

        [OneTimeTearDown]
        public void TearDown()
        {
            CloseBrowser(driver);
        }
    }
}