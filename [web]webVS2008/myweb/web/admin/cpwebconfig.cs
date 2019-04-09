namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpwebconfig : Page
    {
        protected Button btnedit;
        protected RadioButton rbclose;
        protected RadioButton rbopen;
        protected RadioButton rbverifyno;
        protected RadioButton rbverifyyes;
        protected TextBox tbclosetext;
        protected TextBox tbcount;
        protected TextBox tbdescription;
        protected TextBox tbicpinfo;
        protected TextBox tbkeywords;
        protected TextBox tbtitle;

        private void btnedit_Click(object sender, EventArgs e)
        {
            XmlControl control = new XmlControl(base.Server.MapPath("/config/config.config"));
            control.updateContent("config/web/title", this.tbtitle.Text.ToString().Trim());
            control.updateContent("config/web/closetext", this.tbclosetext.Text.ToString().Trim());
            control.updateContent("config/web/icpinfo", this.tbicpinfo.Text.ToString().Trim());
            control.updateContent("config/web/count", this.tbcount.Text.ToString().Trim());
            control.updateContent("config/web/description", this.tbdescription.Text.ToString().Trim());
            control.updateContent("config/web/keywords", this.tbkeywords.Text.ToString().Trim());
            if (this.rbopen.Checked)
            {
                control.updateContent("config/web/open", "true");
            }
            else
            {
                control.updateContent("config/web/open", "false");
            }
            control.Save();
            base.Response.Redirect("cpwebconfig.aspx");
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
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                new system().loadConfig(1);
                this.tbtitle.Text = base.Application["web.title"].ToString();
                this.tbclosetext.Text = base.Application["web.closetext"].ToString();
                this.tbicpinfo.Text = base.Application["web.icpinfo"].ToString();
                this.tbcount.Text = base.Application["web.count"].ToString();
                this.tbdescription.Text = base.Application["web.description"].ToString();
                this.tbkeywords.Text = base.Application["web.keywords"].ToString();
                if (base.Application["web.open"].ToString() == "true")
                {
                    this.rbopen.Checked = true;
                }
                else
                {
                    this.rbclose.Checked = true;
                }
            }
        }
    }
}
