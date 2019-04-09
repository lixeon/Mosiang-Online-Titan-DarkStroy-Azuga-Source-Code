namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.HtmlControls;
    using System.Web.UI.WebControls;
    using web;

    public class baby_login : UserControl
    {
        protected HtmlInputImage Image1;
        protected HtmlInputImage Image2;
        protected TextBox tbuserid;
        protected TextBox tbuserpwd;
        protected TextBox tbverifycode;
        public string weblevel;

        private void Image1_ServerClick(object sender, ImageClickEventArgs e)
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

        private void Image2_ServerClick(object sender, ImageClickEventArgs e)
        {
            base.Session.Abandon();
            base.Response.Redirect("default.aspx");
        }

        private void InitializeComponent()
        {
            this.Image1.ServerClick += new ImageClickEventHandler(this.Image1_ServerClick);
            this.Image2.ServerClick += new ImageClickEventHandler(this.Image2_ServerClick);
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
            }
        }
    }
}
