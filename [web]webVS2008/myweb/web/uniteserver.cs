namespace web
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;

    public class uniteserver : Page
    {
        protected Button btnmodify;
        protected DropDownList DropDownList2;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator2;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbolduserpwd;
        protected TextBox tbuserid;
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
                string userpwd = system.ChkSql(this.tbolduserpwd.Text.ToString());
                string server = "Web_UniteServer" + this.DropDownList2.SelectedValue.ToString();
                str = new WebLogic().uniteserver(userid, userpwd, server);
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
