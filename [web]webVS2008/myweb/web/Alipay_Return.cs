namespace web
{
    using System;
    using System.IO;
    using System.Net;
    using System.Security.Cryptography;
    using System.Text;
    using System.Web.UI;

    public class Alipay_Return : Page
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
            byte[] buffer = new MD5CryptoServiceProvider().ComputeHash(Encoding.GetEncoding("utf-8").GetBytes(s));
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

        protected void Page_Load(object sender, EventArgs e)
        {
            string str = "http://notify.alipay.com/trade/notify_query.do?";
            string str2 = "2088002058515655";
            string str3 = "tb1ogdczyhft9igwywvavb8pzj6bxiy7";
            str = str + "service=notify_verify&partner=" + str2 + "&notify_id=" + base.Request.QueryString["notify_id"];
            string str4 = this.Get_Http(str, 0x1d4c0);
            string[] strArray2 = BubbleSort(base.Request.QueryString.AllKeys);
            string str5 = "";
            for (int i = 0; i < strArray2.Length; i++)
            {
                if (((base.Request.Form[strArray2[i]] != "") && (strArray2[i] != "sign")) && (strArray2[i] != "sign_type"))
                {
                    if (i == (strArray2.Length - 1))
                    {
                        str5 = str5 + strArray2[i] + "=" + base.Request.QueryString[strArray2[i]];
                    }
                    else
                    {
                        str5 = str5 + strArray2[i] + "=" + base.Request.QueryString[strArray2[i]] + "&";
                    }
                }
            }
            string str6 = GetMD5(str5 + str3);
            string str7 = base.Request.QueryString["sign"];
            string tradeno = base.Request.QueryString["out_trade_no"];
            string s = base.Request.QueryString["total_fee"];
            string str10 = base.Request.QueryString["trade_status"];
            int money = (int) float.Parse(s);
            if (((str6 == str7) && (str4 == "true")) && (str10 == "TRADE_FINISHED"))
            {
                base.Response.Write(new WebLogic().alipaydone(tradeno, money));
            }
            else
            {
                base.Response.Write("交易失敗，請拿訂單號找GM");
            }
        }
    }
}
