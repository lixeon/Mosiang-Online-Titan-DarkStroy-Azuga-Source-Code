namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpsecurity : Page
    {
        protected Button btnedit;
        protected RadioButton rbverifyno;
        protected RadioButton rbverifyyes;
        protected TextBox tballowip;
        protected TextBox tbblockip;

        private void btnedit_Click(object sender, EventArgs e)
        {
            XmlControl control = new XmlControl(base.Server.MapPath("/config/config.config"));
            control.updateContent("config/security/cpallowip", this.tballowip.Text.ToString().Trim());
            control.updateContent("config/security/blockip", this.tbblockip.Text.ToString().Trim());
            if (this.rbverifyyes.Checked)
            {
                control.updateContent("config/security/verifycode", "true");
            }
            else
            {
                control.updateContent("config/security/verifycode", "false");
            }
            control.Save();
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
            new WebLogic().isadmin();
            if (!this.Page.IsPostBack)
            {
                new system().loadConfig(1);
                this.tballowip.Text = base.Application["security.cpallowip"].ToString();
                this.tbblockip.Text = base.Application["security.blockip"].ToString();
                if (base.Application["security.verifycode"].ToString() == "true")
                {
                    this.rbverifyyes.Checked = true;
                }
                else
                {
                    this.rbverifyno.Checked = true;
                }
            }
        }
    }
}
