# -*- coding: utf-8 -*-
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys


from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
import unittest, time, re

LOGIN = "admin"
PASSWORD = "Secret123"


class Test(unittest.TestCase):
    def setUp(self):
        #self.driver = webdriver.Firefox()
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub', desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        #self.base_url = "https://ipa.demo1.freeipa.org/"
        self.base_url = "https://fqdn/path/to/app"
        self.verificationErrors = []
        self.accept_next_alert = True

        # prihlaseni uzivatele pred zahajenim testu a otevreni stranky s profilem uzivatele
        self.loginInit()


    # TEST CASE 1 - Test povinne polozky
    def test_1(self):
        driver = self.driver

        if self.is_element_present(By.CSS_SELECTOR, "div[name=\"sn\"] input"):
            try:
                # simulujeme smazani pole uzivatelem
                while driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") != "":
                    driver.find_element_by_css_selector("div[name=\"sn\"] input").send_keys(Keys.BACK_SPACE)

                # pokusime se ulozit
                if driver.find_element_by_css_selector("button[name=\"save\"]").get_attribute("disabled"):
                    self.fail("Error at test case 1 - save button disabled")
                driver.find_element_by_css_selector("button[name=\"save\"]").click()

                # pockame na zobrazeni hlasky
                if not self.wait_for_element(By.CSS_SELECTOR, ".notification-area .alert", 15):
                    self.fail("Error at test case 1 - request timed out")

                # overime, ze se zobrazila varovna hlaska
                if not self.is_element_present(By.CSS_SELECTOR, ".notification-area .alert-danger"):
                    self.fail("Error at test case 1 - no warning displayed")

            except:
                self.fail("Error at test case 1.")
        else:
            self.fail("Error at test case 1 - input \"sn\" not found")

    # TEST CASE 2 - Test neciselne hodnoty ve formulari vyzadujici ciselnou hodnotu
    def test_2(self):
        driver = self.driver
        data = "123abc456"

        if self.is_element_present(By.CSS_SELECTOR, "div[name=\"uidnumber\"] input"):
            try:

                # odstranime puvodni hodnotu
                driver.find_element_by_css_selector("div[name=\"uidnumber\"] input").clear()

                # vlozeni hodnoty
                driver.find_element_by_css_selector("div[name=\"uidnumber\"] input").send_keys(data)

                # pokusime se ulozit
                if driver.find_element_by_css_selector("button[name=\"save\"]").get_attribute("disabled"):
                    self.fail("Error at test case 2 - save button disabled")
                driver.find_element_by_css_selector("button[name=\"save\"]").click()

                # pockame na zobrazeni hlasky
                if not self.wait_for_element(By.CSS_SELECTOR, ".notification-area .alert", 15):
                    self.fail("Error at test case 2 - request timed out")

                # overime, ze se zobrazila varovna hlaska
                if not self.is_element_present(By.CSS_SELECTOR, ".notification-area .alert-danger"):
                    self.fail("Error at test case 2 - no warning displayed")

            except:
                self.fail("Error at test case 2.")
        else:
            self.fail("Error at test case 2 - input \"uidnumber\" not found")

    # TEST CASE 3 - Test vlozeni a overeni hodnot
    def test_3(self):
        driver = self.driver
        val = "abc"
        val2 = "abc1"

        val3 = "bcd"
        val4 = "bcd1"

        try:
            # ziskame aktualni hodnotu
            if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") == val:
                val = val2

            # vycistime input
            driver.find_element_by_css_selector("div[name=\"sn\"] input").clear()

            # vlozime hodnotu
            driver.find_element_by_css_selector("div[name=\"sn\"] input").send_keys(val)

            # ziskame aktualni hodnotu
            if driver.find_element_by_css_selector("div[name=\"cn\"] input").get_attribute("value") == val3:
                val3 = val4

            # vycistime input
            driver.find_element_by_css_selector("div[name=\"cn\"] input").clear()

            # vlozime hodnotu
            driver.find_element_by_css_selector("div[name=\"cn\"] input").send_keys(val3)

            # pokusime se ulozit
            if driver.find_element_by_css_selector("button[name=\"save\"]").get_attribute("disabled"):
                self.fail("Error at test case 3 - save button disabled")
            driver.find_element_by_css_selector("button[name=\"save\"]").click()

            # pockame na zobrazeni hlasky
            if not self.wait_for_element(By.CSS_SELECTOR, ".notification-area .alert", 15):
                self.fail("Error - request timed out")

            # overime, ze se zobrazila uspesna hlaska
            if not self.is_element_present(By.CSS_SELECTOR, ".notification-area .alert-success"):
                self.fail("Error at test case 3 - no message displayed")

            if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") != val and driver.find_element_by_css_selector("div[name=\"cn\"] input").get_attribute("value") != val3:
                self.fail("Error at test case 3 - values do not match")

        except:
            self.fail("Error at test case 3.")

    # TEST CASE 4 - Test ulozeni dlouhe hodnoty
    def test_4(self):
        driver = self.driver
        data = "Administratorbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbdddddddddddddddddddddddddddddddddddddddddddddddgsdgsdgddddddddddddddddddddddddddsggdfgdfg"
        data2 = "Administratorbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbdddddddddddddddddddddddddddddddddddddddddddddddgsdgsdgddddddddddddddddddddddddddsggdfgdff"

        if self.is_element_present(By.CSS_SELECTOR, "div[name=\"sn\"] input"):
            try:

                # zkontrolujeme, zda hodnota jiz neexistuje
                if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") == data:
                    data = data2

                # odstranime puvodni hodnotu
                driver.find_element_by_css_selector("div[name=\"sn\"] input").clear()

                # vlozeni hodnoty
                driver.find_element_by_css_selector("div[name=\"sn\"] input").send_keys(data)

                # pokusime se ulozit
                if driver.find_element_by_css_selector("button[name=\"save\"]").get_attribute("disabled"):
                    self.fail("Error at test case 4 - save button disabled")
                driver.find_element_by_css_selector("button[name=\"save\"]").click()

                # pockame na zobrazeni hlasky
                if not self.wait_for_element(By.CSS_SELECTOR, ".notification-area .alert", 15):
                    self.fail("Error - request timed out")

                # overime, ze se zobrazila uspesna hlaska
                if not self.is_element_present(By.CSS_SELECTOR, ".notification-area .alert-success"):
                    self.fail("Error at test case 4 - no message displayed")

                # porovname vlozenou hodnotu
                if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") != data:
                    self.fail("Error at test case 4 - data not saved")

            except:
                self.fail("Error at test case 4.")
        else:
            self.fail("Error at test case 4 - input \"uidnumber\" not found")

    # TEST CASE 5 - Test prilis velke ciselne hodnoty
    def test_5(self):
        driver = self.driver
        data = "11200000001"

        if self.is_element_present(By.CSS_SELECTOR, "div[name=\"uidnumber\"] input"):
            try:

                # odstranime puvodni hodnotu
                driver.find_element_by_css_selector("div[name=\"uidnumber\"] input").clear()

                # vlozeni hodnoty
                driver.find_element_by_css_selector("div[name=\"uidnumber\"] input").send_keys(data)

                # pokusime se ulozit
                if driver.find_element_by_css_selector("button[name=\"save\"]").get_attribute("disabled"):
                    self.fail("Error at test case 5 - save button disabled")
                driver.find_element_by_css_selector("button[name=\"save\"]").click()

                # pockame na zobrazeni hlasky
                if not self.wait_for_element(By.CSS_SELECTOR, ".notification-area .alert", 15):
                    self.fail("Error at test case 5 - request timed out")

                # overime, ze se zobrazila varovna hlaska
                if not self.is_element_present(By.CSS_SELECTOR, ".notification-area .alert-danger"):
                    self.fail("Error at test case 5 - no warning displayed")

            except:
                self.fail("Error at test case 5.")
        else:
            self.fail("Error at test case 5 - input \"uidnumber\" not found")

    # TEST CASE 6 - Test klavesy TAB
    def test_6(self):
        driver = self.driver

        try:
            # klikneme do formulare
            driver.find_element_by_css_selector("div[name=\"sn\"] input").click()

            # stiskneme klavesu TAB
            driver.find_element_by_css_selector("div[name=\"sn\"] input").send_keys(Keys.TAB)

            # ziskame aktualni element
            element = driver.switch_to.active_element

            # nasledujici element by mel byt se jmenem cn
            if element.get_attribute("name") != "cn":
                self.fail("Error at test case 6 - unexpected TAB behavior.")

        except:
            self.fail("Error at test case 6.")

    # TEST CASE 7 - Test tlacitka Undo
    def test_7(self):
        driver = self.driver
        data = "test"
        data2 = "testt"

        try:
            if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") == data:
                data = data2

            # vycistime pole
            driver.find_element_by_css_selector("div[name=\"sn\"] input").clear()

            # vlozime hodnotu
            driver.find_element_by_css_selector("div[name=\"sn\"] input").send_keys(data)

            # overeni vlozeni dat do formulare
            if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") != data:
                self.fail("Error at test case 7 - value not set")

            # pouzijeme tlacitko undo
            driver.find_element_by_css_selector("div[name=\"sn\"] button").click()

            # data musi byt ruzna od testovacich
            if driver.find_element_by_css_selector("div[name=\"sn\"] input").get_attribute("value") == data:
                self.fail("Error at test case 7 - undo button fail")

        except:
            self.fail("Error at test case 7.")

    # prihlaseni uzivatele a zobrazeni stranky s profilem
    def loginInit(self):
        driver = self.driver
        driver.get("https://ipa.demo1.freeipa.org/ipa/ui/")

        # pockame nez se objevi formular
        if not self.wait_for_element(By.CSS_SELECTOR, "#username1", 15):
            self.fail("Error at login user into application - request timed out")

        # pokusime se prihlasit uzivatele a zobrazit stranku
        try:
            driver.find_element_by_css_selector("#username1").send_keys(LOGIN)
            driver.find_element_by_css_selector("#password2").send_keys(PASSWORD)
            driver.find_element_by_css_selector("button[name=\"login\"]").click()
        except:
            self.fail("Error at login user into application.")

        # klikneme na prislusneho uzivatele v tabulce a zobrazime stranku s profilem
        try:
            driver.find_element_by_css_selector(".content-table").find_element_by_partial_link_text(LOGIN).click()
        except:
            self.fail("Could not display user profile")

        # pockame nez se dokonci nacitani stranky
        if not self.wait_for_element(By.CSS_SELECTOR, ".global-activity-indicator.closed", 15):
            self.fail("Error - request timed out")

    # funkce pocka na prvek nez se nacte
    def wait_for_element(self, selector, name, seconds):
        if seconds >= 0:
            for i in range(seconds):
                try:
                    if self.is_element_present(selector, name):
                        return True
                except:
                    pass
                time.sleep(seconds)
        return False

    def is_element_present(self, how, what):
        try:
            self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e:
            return False
        return True
    
    def is_alert_present(self):
        try:
            self.driver.switch_to_alert()
        except NoAlertPresentException as e:
            return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally:
            self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)

if __name__ == "__main__":
    unittest.main()
