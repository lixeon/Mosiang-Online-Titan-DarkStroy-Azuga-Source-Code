namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class login : UserControl
    {
        protected Button btnlogin;
        protected Button Button1;
        protected TextBox tbuserid;
        protected TextBox tbuserpwd;
        protected TextBox tbverifycode;
        public string weblevel;

        private void btnlogin_Click(object sender, EventArgs e)
        {
            string userid = new system().ChkSql(this.tbuserid.Text.ToString().Trim());
            string userpwd = new system().ChkSql(this.tbuserpwd.Text.ToString().Trim());
            string str3 = this.tbverifycode.Text.ToString().Trim();
            if ((userid == "") || (userpwd == ""))
            {
                base.Response.Write("<script language=javascript>alert(\"帳號密碼不可為空!\")</script>");
            }
            else if (str3 != base.Session["verifycode"].ToString())
            {
                base.Response.Write("<script language=javascript>alert(\"驗證碼輸入錯誤，請重新核對\")</script>");
            }
            else if (new WebLogic().login(userid, userpwd))
            {
                if (base.Request.QueryString["returnurl"] != null)
                {
                    base.Response.Redirect(base.Request.QueryString["returnurl"].ToString());
                }
                else
                {
                    base.Response.Redirect("default.aspx");
                }
            }
            else
            {
                base.Response.Write("<script language=javascript>alert(\"帳號或密碼錯誤，請重新核對\")</script>");
            }
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            base.Session.Abandon();
            base.Response.Redirect("default.aspx");
        }

        private void InitializeComponent()
        {
            this.btnlogin.Click += new EventHandler(this.btnlogin_Click);
            this.Button1.Click += new EventHandler(this.Button1_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
            if (base.Session["userid"] != null)
            {
                if (base.Session["weblevel"].ToString() == "0")
                {
                    this.weblevel = "普通會員";
                }
                else if (base.Session["weblevel"].ToString() == "1")
                {
                    this.weblevel = "黃金會員";
                }
                else if (base.Session["weblevel"].ToString() == "2")
                {
                    this.weblevel = "白金會員";
                }
                else if (base.Session["weblevel"].ToString() == "3")
                {
                    this.weblevel = "鑽石會員";
                }
            }
        }
    }
}
