namespace web
{
    using System;
    using System.IO;
    using System.Security.Cryptography;
    using System.Text;
    using System.Web;
    using System.Web.UI;

    public class system : UserControl
    {
        private static char[] hexDigits = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        public bool checkIP()
        {
            try
            {
                string content = new XmlControl(HttpContext.Current.Server.MapPath("~") + @"\config\config.config").GetContent("config/security/cpallowip");
                string clientIP = this.GetClientIP();
                string[] strArray = content.Split(new char[] { '|' });
                for (int i = 0; i < strArray.Length; i++)
                {
                    string[] strArray2 = strArray[i].Split(new char[] { '.' });
                    string[] strArray3 = clientIP.Split(new char[] { '.' });
                    int num2 = 4 - this.strCount(strArray[i], "*");
                    int num3 = 0;
                    for (int j = 0; j < strArray2.Length; j++)
                    {
                        if ((strArray2[j] != "*") && (strArray2[j] == strArray3[j]))
                        {
                            num3++;
                        }
                    }
                    if (num3 == num2)
                    {
                        return true;
                    }
                }
            }
            catch (Exception exception)
            {
                throw exception;
            }
            return false;
        }

        public string ChkSql(string str)
        {
            str = str.Replace("'", "");
            return str;
        }

        public string ConvertToBig5(string str, int mode)
        {
            if ((str == null) || !(str != ""))
            {
                return str;
            }
            if (mode == 0x3a8)
            {
                return (str = Encoding.Default.GetString(Encoding.Convert(Encoding.GetEncoding(0x3a8), Encoding.GetEncoding(950), Encoding.Default.GetBytes(str))));
            }
            return (str = Encoding.Default.GetString(Encoding.Convert(Encoding.GetEncoding(950), Encoding.GetEncoding(0x3a8), Encoding.Default.GetBytes(str))));
        }

        public string Decrypt(string pToDecrypt, string sKey)
        {
            DESCryptoServiceProvider provider = new DESCryptoServiceProvider();
            byte[] buffer = new byte[pToDecrypt.Length / 2];
            for (int i = 0; i < (pToDecrypt.Length / 2); i++)
            {
                int num2 = Convert.ToInt32(pToDecrypt.Substring(i * 2, 2), 0x10);
                buffer[i] = (byte) num2;
            }
            provider.Key = Encoding.ASCII.GetBytes(sKey);
            provider.IV = Encoding.ASCII.GetBytes(sKey);
            MemoryStream stream = new MemoryStream();
            CryptoStream stream2 = new CryptoStream(stream, provider.CreateDecryptor(), CryptoStreamMode.Write);
            stream2.Write(buffer, 0, buffer.Length);
            stream2.FlushFinalBlock();
            new StringBuilder();
            return Encoding.Default.GetString(stream.ToArray());
        }

        public void defCC()
        {
            if (HttpContext.Current.Request.ServerVariables["HTTP_VIA"] != null)
            {
                HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"].ToString();
                if (HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"] == null)
                {
                    HttpContext.Current.Response.Redirect("error.html");
                }
                if (HttpContext.Current.Request.ServerVariables["HTTP_VIA"] == HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"])
                {
                    HttpContext.Current.Response.Redirect("error.html");
                }
            }
            else
            {
                HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"].ToString();
                if (HttpContext.Current.Request.ServerVariables["Remote_Host"] != HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"])
                {
                    HttpContext.Current.Response.Redirect("error.html");
                }
            }
        }

        public string Encrypt(string pToEncrypt, string sKey)
        {
            DESCryptoServiceProvider provider = new DESCryptoServiceProvider();
            byte[] bytes = Encoding.Default.GetBytes(pToEncrypt);
            provider.Key = Encoding.ASCII.GetBytes(sKey);
            provider.IV = Encoding.ASCII.GetBytes(sKey);
            MemoryStream stream = new MemoryStream();
            CryptoStream stream2 = new CryptoStream(stream, provider.CreateEncryptor(), CryptoStreamMode.Write);
            stream2.Write(bytes, 0, bytes.Length);
            stream2.FlushFinalBlock();
            StringBuilder builder = new StringBuilder();
            foreach (byte num in stream.ToArray())
            {
                builder.AppendFormat("{0:X2}", num);
            }
            builder.ToString();
            return builder.ToString();
        }

        public string GetClientIP()
        {
            string str = HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"];
            switch (str)
            {
                case null:
                case "":
                    str = HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"];
                    break;
            }
            if ((str != null) && !(str == string.Empty))
            {
                return str;
            }
            return HttpContext.Current.Request.UserHostAddress;
        }

        public int getStrLen(string str)
        {
            return Encoding.GetEncoding("BIG5").GetBytes(str.ToCharArray()).Length;
        }

        public bool IsNum(string str)
        {
            for (int i = 0; i < str.Length; i++)
            {
                if (!char.IsNumber(str, i))
                {
                    return false;
                }
            }
            return true;
        }

        public void loadConfig(int read)
        {
            this.defCC();
            if ((HttpContext.Current.Application["loadconfig"] == null) || (read == 1))
            {
                XmlControl control = new XmlControl(HttpContext.Current.Server.MapPath("~") + @"\config\config.config");
                HttpContext.Current.Application.Clear();
                HttpContext.Current.Application.RemoveAll();
                HttpContext.Current.Application.Add("web.open", control.GetContent("config/web/open"));
                HttpContext.Current.Application.Add("web.url", control.GetContent("config/web/url"));
                HttpContext.Current.Application.Add("web.forum", control.GetContent("config/web/forum"));
                HttpContext.Current.Application.Add("web.servername", control.GetContent("config/web/servername"));
                HttpContext.Current.Application.Add("web.title", control.GetContent("config/web/title"));
                HttpContext.Current.Application.Add("web.keywords", control.GetContent("config/web/keywords"));
                HttpContext.Current.Application.Add("web.description", control.GetContent("config/web/description"));
                HttpContext.Current.Application.Add("web.count", control.GetContent("config/web/count"));
                HttpContext.Current.Application.Add("web.icpinfo", control.GetContent("config/web/icpinfo"));
                HttpContext.Current.Application.Add("web.closetext", control.GetContent("config/web/closetext"));
                HttpContext.Current.Application.Add("alipay.email", control.GetContent("config/alipay/email"));
                HttpContext.Current.Application.Add("alipay.partner", control.GetContent("config/alipay/partner"));
                HttpContext.Current.Application.Add("alipay.key", control.GetContent("config/alipay/key"));
                if (HttpContext.Current.Application["web.open"].ToString() == "false")
                {
                    return;
                }
                HttpContext.Current.Application.Add("db.debug", control.GetContent("config/db/debug"));
                HttpContext.Current.Application.Add("db.connect", this.Decrypt(control.GetContent("config/db/connect"), "wfwfwfwf"));
                DataProviders providers = new DataProviders();
                HttpContext.Current.Application.Add("game.modifychanamecount", control.GetContent("config/game/modifychanamecount"));
                HttpContext.Current.Application.Add("game.modifychanamemoney", control.GetContent("config/game/modifychanamemoney"));
                HttpContext.Current.Application.Add("game.charesetcount", control.GetContent("config/game/charesetcount"));
                HttpContext.Current.Application.Add("game.charesetgivepoint", control.GetContent("config/game/charesetgivepoint"));
                HttpContext.Current.Application.Add("game.charesetflv", control.GetContent("config/game/charesetflv"));
                HttpContext.Current.Application.Add("game.charesetlvstep", control.GetContent("config/game/charesetlvstep"));
                HttpContext.Current.Application.Add("game.charesetfmoney", control.GetContent("config/game/charesetfmoney"));
                HttpContext.Current.Application.Add("game.charesetmoneystep", control.GetContent("config/game/charesetmoneystep"));
                HttpContext.Current.Application.Add("game.commendmoney", control.GetContent("config/game/commendmoney"));
                HttpContext.Current.Application.Add("game.commendgold", control.GetContent("config/game/commendgold"));
                HttpContext.Current.Application.Add("game.commendreborn", control.GetContent("config/game/commendreborn"));
                HttpContext.Current.Application.Add("game.clearpkgold", control.GetContent("config/game/clearpkgold"));
                HttpContext.Current.Application.Add("game.petlvupgold2", control.GetContent("config/game/petlvupgold2"));
                HttpContext.Current.Application.Add("game.petlvupgold3", control.GetContent("config/game/petlvupgold3"));
                HttpContext.Current.Application.Add("game.skilllvupmoney", control.GetContent("config/game/skilllvupmoney"));
                HttpContext.Current.Application.Add("game.goldtomoney", control.GetContent("config/game/goldtomoney"));
                HttpContext.Current.Application.Add("game.vipoffer", control.GetContent("config/game/vipoffer"));
                HttpContext.Current.Application.Add("game.resetpointmoney", control.GetContent("config/game/resetpointmoney"));
                HttpContext.Current.Application.Add("game.resetpointgold", control.GetContent("config/game/resetpointgold"));
                HttpContext.Current.Application.Add("game.pssigngold", control.GetContent("config/game/pssigngold"));
                HttpContext.Current.Application.Add("game.changeinfogold", control.GetContent("config/game/changeinfogold"));
                HttpContext.Current.Application.Add("security.cpallowip", control.GetContent("config/security/cpallowip"));
                HttpContext.Current.Application.Add("security.blockip", control.GetContent("config/security/blockip"));
                HttpContext.Current.Application.Add("security.verifycode", control.GetContent("config/security/verifycode"));
                HttpContext.Current.Application.Add("register.allow", control.GetContent("config/register/allow"));
                HttpContext.Current.Application.Add("register.givegold", control.GetContent("config/register/givegold"));
                HttpContext.Current.Application.Add("register.stopregtext", control.GetContent("config/register/stopregtext"));
                HttpContext.Current.Application.Add("notice", providers.ExecScalarOne("select notice from web_notice "));
                HttpContext.Current.Application.Add("sell", providers.ExecScalarOne("select sell from web_notice "));
                HttpContext.Current.Application.Add("commendtext", providers.ExecScalarOne("select commend from mhcmember..web_notice"));
                HttpContext.Current.Application.Add("loadconfig", true);
            }
            if (HttpContext.Current.Request.QueryString["sid"] != null)
            {
                HttpContext.Current.Session["sid"] = "allowArea";
            }
            if (new AllowIP().ScopeIncludingIpAddress(this.GetClientIP()) && (HttpContext.Current.Request.QueryString["sid"] == null))
            {
                if (HttpContext.Current.Application["web.servername"].ToString().Equals("[墨香世界]"))
                {
                    HttpContext.Current.Response.Redirect("http://www.skymso.com/notice.aspx", true);
                }
                if (HttpContext.Current.Application["web.servername"].ToString().Equals("[寶寶墨香]"))
                {
                    HttpContext.Current.Response.Redirect("http://www.50mso.com/notice.aspx", true);
                }
            }
            if ((HttpContext.Current.Application["web.servername"].ToString().Equals("[51墨香]") && !new AllowIP().ScopeIncludingIpAddressx(this.GetClientIP())) && (HttpContext.Current.Request.QueryString["sid"] == null))
            {
                HttpContext.Current.Response.Redirect("http://www.50mso.com/notice.aspx", true);
            }
        }

        public string NumToHex(long v, int len)
        {
            string str = "";
            int startIndex = 0;
            string str2 = Convert.ToString(v, 0x10);
            int length = str2.Length;
            if ((length % 2) == 1)
            {
                length++;
                str2 = "0" + str2;
            }
            for (int i = 0; i < length; i += 2)
            {
                startIndex = (length - i) - 2;
                str = str + str2.Substring(startIndex, 2);
            }
            for (int j = 0; j < (len - length); j++)
            {
                str = str + "0";
            }
            return str.ToUpper();
        }

        public int strCount(string str, string replacestr)
        {
            return (str.Length - str.Replace(replacestr, "").Length);
        }

        public string ToHexString(byte[] bytes)
        {
            char[] chArray = new char[bytes.Length * 2];
            for (int i = 0; i < bytes.Length; i++)
            {
                int num2 = bytes[i];
                chArray[i * 2] = hexDigits[num2 >> 4];
                chArray[(i * 2) + 1] = hexDigits[num2 & 15];
            }
            return new string(chArray);
        }
    }
}
