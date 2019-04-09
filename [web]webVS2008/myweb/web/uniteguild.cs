namespace web
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;

    public class uniteguild : Page
    {
        protected Button btnmodify;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator3;
        protected RegularExpressionValidator RegularExpressionValidator4;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator2;
        protected RequiredFieldValidator Requiredfieldvalidator3;
        protected RequiredFieldValidator Requiredfieldvalidator4;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected RequiredFieldValidator Requiredfieldvalidator7;
        protected TextBox tbnewchaname;
        protected TextBox tboldchaname;
        protected TextBox tbolduserid;
        protected TextBox tbolduserpwd;
        protected TextBox tbuserid;
        protected TextBox tbuserpwd;
        protected TextBox tbverifycode;

        private void btnmodify_Click(object sender, EventArgs e)
        {
            string str = "";
            if (this.tbverifycode.Text != this.Session["VerifyCode"].ToString())
            {
                str = "驗證碼錯誤！";
            }
            else
            {
                system system = new system();
                string userid = system.ChkSql(this.tbuserid.Text.ToString());
                string userpwd = system.ChkSql(this.tbuserpwd.Text.ToString());
                string olduserid = system.ChkSql(this.tbolduserid.Text.ToString());
                string olduserpwd = system.ChkSql(this.tbolduserpwd.Text.ToString());
                string newchaname = system.ConvertToBig5(system.ChkSql(this.tbnewchaname.Text.ToString()), 0x3a8);
                string oldchaname = system.ConvertToBig5(system.ChkSql(this.tboldchaname.Text.ToString()), 0x3a8);
                str = new WebLogic().uniteguild(userid, userpwd, newchaname, olduserid, olduserpwd, oldchaname);
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
