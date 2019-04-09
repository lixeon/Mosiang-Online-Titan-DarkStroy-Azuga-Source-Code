namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.Security;
    using System.Web.UI;
    using System.Web.UI.HtmlControls;
    using System.Web.UI.WebControls;
    using web;

    public class admincp : Page
    {
        protected HtmlInputText admin_id;
        protected HtmlInputText admin_pwd;
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
            string adminid = system.ChkSql(this.admin_id.Value.ToString());
            string password = system.ChkSql(this.admin_pwd.Value.ToString());
            if (this.vcode.Value != this.Session["VerifyCode"].ToString())
            {
                base.Response.Write("<script language=javascript>alert(\"驗證碼錯誤！\")</script>");
            }
            else
            {
                logic.log("", "", adminid, 0, "登陸後臺", this.Page.Request.UserHostAddress.ToString(), "後台登陸日誌");
                logic.log("", "", adminid, 0, "登陸後臺", system.GetClientIP(), "後台登陸日誌");
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..web_login where userid='" + adminid + "' and password='" + FormsAuthentication.HashPasswordForStoringInConfigFile(password, "MD5") + "' and state=1");
                if (reader.Read())
                {
                    this.Session.Timeout = 600;
                    this.Session["admin_id"] = reader["userid"].ToString();
                    this.Session["admin_name"] = reader["name"].ToString();
                    this.Session["admin_ip"] = system.GetClientIP();
                    base.Response.Redirect("cpserverinfo.aspx");
                }
                else
                {
                    base.Response.Write("<script language=javascript>alert(\"用戶名或密碼錯誤！\")</script>");
                }
                providers.CloseConn();
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
        }
    }
}
