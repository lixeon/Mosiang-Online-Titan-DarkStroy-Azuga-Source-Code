namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class changeinfo : UserControl
    {
        protected Button btnedit;
        protected CompareValidator CompareValidator1;
        protected CompareValidator CompareValidator2;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator3;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RequiredFieldValidator Requiredfieldvalidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator Requiredfieldvalidator3;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbemail;
        protected TextBox tbkey;
        protected TextBox tbreemail;
        protected TextBox tbrekey;
        protected TextBox tbuserid;
        protected TextBox tbverifycode;

        private void btnedit_Click(object sender, EventArgs e)
        {
            string userid = base.Session["userid"].ToString();
            string key = new system().ChkSql(this.tbkey.Text.ToString());
            string email = new system().ChkSql(this.tbemail.Text.ToString());
            int changeinfogold = int.Parse(base.Application["game.changeinfogold"].ToString());
            string str4 = new WebLogic().changeinfo(userid, key, email, changeinfogold, new system().GetClientIP());
            base.Response.Write("<script language=javascript>alert('" + str4 + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
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
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
        }
    }
}
