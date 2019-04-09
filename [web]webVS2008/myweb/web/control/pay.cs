namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class pay : UserControl
    {
        protected Button btncreate;
        protected Button btnpay;
        protected DropDownList ddtradetype;
        protected Label lbltradeno;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator3;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator2;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbuserid;
        protected TextBox tbverifycode;

        private string alipay(string out_trade_no, string out_body, int money)
        {
            string gateway = "https://www.alipay.com/cooperate/gateway.do?";
            string service = "create_direct_pay_by_user";
            string partner = base.Application["alipay.partner"].ToString();
            string str4 = "MD5";
            string subject = out_trade_no;
            string body = out_body;
            string str7 = "1";
            string str8 = money.ToString();
            string str9 = "http://" + base.Application["web.url"];
            string str10 = base.Application["alipay.email"].ToString();
            string key = base.Application["alipay.key"].ToString();
            string str12 = "http://" + base.Application["web.url"] + "/Alipay_Notify.aspx";
            string paymethod = "bankPay";
            AliPay pay = new AliPay();
            return pay.CreatUrl(gateway, service, partner, str4, out_trade_no, subject, body, str7, str8, str9, str10, key, str12, paymethod);
        }

        private void btncreate_Click(object sender, EventArgs e)
        {
            if (this.tbverifycode.Text.ToString() != base.Session["verifycode"].ToString())
            {
                base.Response.Write("<script language=javascript>alert(\"驗證碼輸入錯誤，請重新核對\")</script>");
            }
            else
            {
                string userid = new system().ChkSql(this.tbuserid.Text.ToString().Trim());
                string tradeno = this.createTradeno();
                int money = int.Parse(this.ddtradetype.SelectedValue.ToString());
                string str3 = this.ddtradetype.SelectedItem.ToString();
                string str4 = new WebLogic().alipaysubmit(userid, tradeno, money);
                if (str4 == "success")
                {
                    this.lbltradeno.Visible = true;
                    this.btnpay.Visible = true;
                    this.btncreate.Visible = false;
                    this.tbverifycode.Text = "";
                    this.lbltradeno.Text = "你的定單號是" + tradeno + ",購買物品：" + str3 + "，請抄下訂單號";
                    base.Session["tradeurl"] = this.alipay(tradeno, str3, money);
                }
                else
                {
                    base.Response.Write("<script language=javascript>alert('" + str4 + "')</script>");
                }
            }
        }

        private void btnpay_Click(object sender, EventArgs e)
        {
            if (this.tbverifycode.Text.ToString() != base.Session["verifycode"].ToString())
            {
                base.Response.Write("<script language=javascript>alert(\"驗證碼輸入錯誤，請重新核對\")</script>");
            }
            else
            {
                string url = base.Session["tradeurl"].ToString();
                base.Session["tradeurl"] = "";
                base.Response.Redirect(url);
            }
        }

        private string createTradeno()
        {
            Random random = new Random();
            DateTime time = new DateTime();
            string str = DateTime.Now.ToString("u").Replace("-", "").Replace(":", "").Replace(" ", "").Replace("Z", "");
            random.Next(10, 0x63);
            return (str + random.Next(10, 0x63).ToString());
        }

        private void InitializeComponent()
        {
            this.btncreate.Click += new EventHandler(this.btncreate_Click);
            this.btnpay.Click += new EventHandler(this.btnpay_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["tradeno"] == null)
            {
                this.lbltradeno.Visible = false;
                this.btnpay.Visible = false;
                this.btncreate.Visible = true;
                this.lbltradeno.Text = "";
            }
            else if (base.Session["tradeno"].ToString() == "")
            {
                this.lbltradeno.Visible = false;
                this.btnpay.Visible = false;
                this.btncreate.Visible = true;
                this.lbltradeno.Text = "";
            }
        }
    }
}
