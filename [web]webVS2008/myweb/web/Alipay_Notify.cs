namespace web
{
    using System;
    using System.IO;
    using System.Net;
    using System.Security.Cryptography;
    using System.Text;
    using System.Web.UI;

    public class Alipay_Notify : Page
    {
        public static string[] BubbleSort(string[] r)
        {
            for (int i = 0; i < r.Length; i++)
            {
                bool flag = false;
                for (int j = r.Length - 2; j >= i; j--)
                {
                    if (string.CompareOrdinal(r[j + 1], r[j]) < 0)
                    {
                        string str = r[j + 1];
                        r[j + 1] = r[j];
                        r[j] = str;
                        flag = true;
                    }
                }
                if (!flag)
                {
                    return r;
                }
            }
            return r;
        }

        public string Get_Http(string a_strUrl, int timeout)
        {
            try
            {
                HttpWebRequest request = (HttpWebRequest) WebRequest.Create(a_strUrl);
                request.Timeout = timeout;
                HttpWebResponse response = (HttpWebResponse) request.GetResponse();
                StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.Default);
                StringBuilder builder = new StringBuilder();
                while (-1 != reader.Peek())
                {
                    builder.Append(reader.ReadLine());
                }
                return builder.ToString();
            }
            catch (Exception exception)
            {
                return ("错误：" + exception.Message);
            }
        }

        public static string GetMD5(string s)
        {
            byte[] buffer = new MD5CryptoServiceProvider().ComputeHash(Encoding.GetEncoding("big5").GetBytes(s));
            StringBuilder builder = new StringBuilder(0x20);
            for (int i = 0; i < buffer.Length; i++)
            {
                builder.Append(buffer[i].ToString("x").PadLeft(2, '0'));
            }
            return builder.ToString();
        }

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            string str = "http://notify.alipay.com/trade/notify_query.do?";
            string str2 = base.Application["alipay.partner"].ToString();
            string str3 = base.Application["alipay.key"].ToString();
            str = str + "&partner=" + str2 + "&notify_id=" + base.Request.Form["notify_id"];
            string str4 = this.Get_Http(str, 0x1d4c0);
            string[] strArray2 = BubbleSort(base.Request.Form.AllKeys);
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < strArray2.Length; i++)
            {
                if ((strArray2[i] != "sign") && (strArray2[i] != "sign_type"))
                {
                    if (i == (strArray2.Length - 1))
                    {
                        builder.Append(strArray2[i] + "=" + base.Request.Form[strArray2[i]]);
                    }
                    else
                    {
                        builder.Append(strArray2[i] + "=" + base.Request.Form[strArray2[i]] + "&");
                    }
                }
            }
            builder.Append(str3);
            string str5 = GetMD5(builder.ToString());
            string str6 = base.Request.Form["sign"];
            string tradeno = base.Request.Form["out_trade_no"];
            string s = base.Request.Form["total_fee"];
            string str9 = base.Request.Form["trade_status"];
            int money = (int) float.Parse(s);
            if (((str5 == str6) && (str4 == "true")) && (str9 == "TRADE_FINISHED"))
            {
                new WebLogic().alipaydone(tradeno, money);
                base.Response.Write("success");
                base.Response.Write("<br>------------------" + base.Request.Form["body"] + "--------------成功了");
            }
            else
            {
                base.Response.Write("fail");
                base.Response.Write("<br>------------------" + base.Request.Form["body"]);
            }
        }
    }
}
