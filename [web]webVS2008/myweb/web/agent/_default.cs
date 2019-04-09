namespace web.agent
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.HtmlControls;
    using System.Web.UI.WebControls;
    using web;

    public class _default : Page
    {
        protected HtmlInputText agent_id;
        protected HtmlInputText agent_pwd;
        protected HtmlInputButton btnLogin;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator3;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator RequiredFieldValidator3;
        protected HtmlInputText vcode;

        private void btnLogin_ServerClick(object sender, EventArgs e)
        {
            DataProviders providers = new DataProviders();
            system system = new system();
            WebLogic logic = new WebLogic();
            string agentid = system.ChkSql(this.agent_id.Value.ToString());
            string str2 = system.ChkSql(this.agent_pwd.Value.ToString());
            if (this.vcode.Value != this.Session["VerifyCode"].ToString())
            {
                base.Response.Write("<script language=javascript>alert(\"驗證碼錯誤！\")</script>");
            }
            else
            {
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..web_agent where userid='" + agentid + "' and password='" + str2 + "' and state=1");
                if (reader.Read())
                {
                    this.Session.Timeout = 600;
                    this.Session["agent_id"] = reader["userid"].ToString();
                    this.Session["agent_name"] = reader["name"].ToString();
                    this.Session["agent_ip"] = system.GetClientIP();
                    logic.log("", agentid, "", 0, "代理後台登陸成功", system.GetClientIP(), "代理登陸日誌");
                    base.Response.Redirect("account.aspx");
                }
                else
                {
                    logic.log("", "", agentid, 0, "用戶名或密碼錯誤ID：" + agentid + "PW：" + str2, system.GetClientIP(), "代理登陸日誌");
                    base.Response.Write("<script language=javascript>alert(\"用戶名或密碼錯誤！\")</script>");
                }
            }
        }

        private void InitializeComponent()
        {
            this.btnLogin.ServerClick += new EventHandler(this.btnLogin_ServerClick);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            this.Session["sid"] = "ok";
            new system().loadConfig(0);
        }
    }
}
