namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpregister : Page
    {
        protected Button btnedit;
        protected RadioButton rbregno;
        protected RadioButton rbregyes;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected TextBox tbgivegold;
        protected TextBox tbstopreg;

        private void btnedit_Click(object sender, EventArgs e)
        {
            XmlControl control = new XmlControl(base.Server.MapPath("/config/config.config"));
            control.updateContent("config/register/stopregtext", this.tbstopreg.Text.ToString().Trim());
            control.updateContent("config/register/givegold", this.tbgivegold.Text.ToString().Trim());
            if (this.rbregyes.Checked)
            {
                control.updateContent("config/register/allow", "true");
            }
            else
            {
                control.updateContent("config/register/allow", "false");
            }
            control.Save();
            new system().loadConfig(1);
            base.Response.Write("<script>alert('修改成功')</script>");
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
            new WebLogic().isadmin();
            if (!this.Page.IsPostBack)
            {
                new system().loadConfig(1);
                this.tbstopreg.Text = base.Application["register.stopregtext"].ToString();
                this.tbgivegold.Text = base.Application["register.givegold"].ToString();
                if (base.Application["register.allow"].ToString() == "true")
                {
                    this.rbregyes.Checked = true;
                }
                else
                {
                    this.rbregno.Checked = true;
                }
            }
        }
    }
}
