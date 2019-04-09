namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class modifypassword : UserControl
    {
        protected Button btnmodify;
        protected CompareValidator CompareValidator1;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator4;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbnewuserpwd;
        protected TextBox tbolduserpwd;
        protected TextBox tbrenewuserpwd;
        protected TextBox tbuserid;
        protected TextBox tbverifycode;

        private void btnmodify_Click(object sender, EventArgs e)
        {
            string str = "";
            if (this.tbverifycode.Text != base.Session["VerifyCode"].ToString())
            {
                str = "驗證碼錯誤！";
            }
            else
            {
                system system = new system();
                string userid = system.ChkSql(this.tbuserid.Text.ToString());
                string olduserpwd = system.ChkSql(this.tbolduserpwd.Text.ToString());
                string newuserpwd = system.ChkSql(this.tbnewuserpwd.Text.ToString());
                str = new WebLogic().modifypassword(userid, olduserpwd, newuserpwd);
            }
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnmodify.Click += new EventHandler(this.btnmodify_Click);
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
        }
    }
}
