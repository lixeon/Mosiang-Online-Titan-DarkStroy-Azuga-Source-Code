namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpserverinfo : Page
    {
        protected Button btnread;
        protected Button btnreadonline;
        protected Label lbl2lvpet;
        protected Label lbl3lvpet;
        protected Label lblagentgold;
        protected Label lblallcharacter;
        protected Label lblbank;
        protected Label lblfemale;
        protected Label lblmale;
        protected Label lblonlineplayer;
        protected Label lblplayergold;
        protected Label lblregister;
        protected Label lblvip;

        private void btnread_Click(object sender, EventArgs e)
        {
            DataProviders providers = new DataProviders();
            this.lblonlineplayer.Text = providers.ExecScalarOne("select sum(1) from mhcmember..logintable");
            this.lblregister.Text = providers.ExecScalarOne("select sum(1) from mhcmember..chr_log_info");
            this.lblmale.Text = providers.ExecScalarOne("select sum(1) from mhcmember..chr_log_info where websex='帥哥'");
            this.lblfemale.Text = providers.ExecScalarOne("select sum(1) from mhcmember..chr_log_info where websex='美女'");
            this.lblvip.Text = providers.ExecScalarOne("select sum(1) from mhcmember..chr_log_info where weblevel=1");
            this.lbl2lvpet.Text = providers.ExecScalarOne("select sum(1) from mhgame..tb_petinfo where grade=2");
            this.lbl3lvpet.Text = providers.ExecScalarOne("select sum(1) from mhgame..tb_petinfo where grade=3");
            this.lblbank.Text = providers.ExecScalarOne("select sum(webbank) from mhcmember..chr_log_info");
            this.lblplayergold.Text = string.Format("{0:N}", providers.ExecScalarOne("select sum(webgold) from mhcmember..chr_log_info"));
            this.lblagentgold.Text = providers.ExecScalarOne("select sum(gold) from mhcmember..web_agent");
            this.lblallcharacter.Text = providers.ExecScalarOne("select sum(1) from mhgame..tb_character");
        }

        private void btnreadonline_Click(object sender, EventArgs e)
        {
            this.lblonlineplayer.Text = new DataProviders().ExecScalarOne("select sum(1) from mhcmember..logintable");
        }

        private void InitializeComponent()
        {
            this.btnread.Click += new EventHandler(this.btnread_Click);
            this.btnreadonline.Click += new EventHandler(this.btnreadonline_Click);
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
        }
    }
}
