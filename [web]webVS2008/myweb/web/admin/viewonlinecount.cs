namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class viewonlinecount : Page
    {
        protected Label Label1;
        protected Label lblonlineplayer;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            DataProviders providers = new DataProviders();
            this.lblonlineplayer.Text = providers.ExecScalarOne("select sum(1) from mhcmember..logintable");
            this.Label1.Text = providers.ExecScalarOne("select sum(gold) as gold from web_log where type='代理發放金幣' and convert(char(10),date,120)=convert(char(10),getdate(),120)");
        }
    }
}
